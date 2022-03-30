#include "rex_diagnostics_pch.h"

#include "profiling/instrumentor.h"

#include "logging/logger.h"

namespace rex
{
    //--------------------------------------------------------------------------------------------
    Instrumentor::Instrumentor()
        : m_current_session(nullptr)
        , m_enabled(false)
    {
    }

    //--------------------------------------------------------------------------------------------
    void Instrumentor::enable()
    {
        m_enabled = true;
    }
    //--------------------------------------------------------------------------------------------
    void Instrumentor::disable()
    {
        m_enabled = false;
    }
    //--------------------------------------------------------------------------------------------
    bool Instrumentor::is_enabled() const
    {
        return m_enabled;
    }

    //--------------------------------------------------------------------------------------------
    void Instrumentor::begin_session(const std::string& name, const std::string& filepath)
    {
        if (!m_enabled)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_current_session)
        {
            // If there is already a current session, then close it before beginning new one.
            // Subsequent profiling output meant for the original session will end up in the
            // newly opened session instead.  That's better than having badly formatted
            // profiling output.
            if (rex::logging::has_logger(rex::logging::tags::ENGINE_LOGGER_NAME))
            {
                // Edge case: begin_session() might be before Log::Init()
                rex::logging::get_logger(rex::logging::tags::ENGINE_LOGGER_NAME).error("[INSTRUMENTOR] Instrumentor::begin_session('{0}') when session '{1}' already open.", name, m_current_session->name);
            }

            internal_end_session();
        }

        rex::logging::get_logger(rex::logging::tags::ENGINE_LOGGER_NAME).info("[INSTRUMENTOR] Starting session: {0} @ {1}", name, filepath);
        m_output_stream.open(filepath);
        if (m_output_stream.is_open())
        {
            m_current_session = new InstrumentationSession({name});

            write_header();
        }
        else if (rex::logging::has_logger(rex::logging::tags::ENGINE_LOGGER_NAME))
        {
            // Edge case: BeginSession() might be before Log::Init()
            rex::logging::get_logger(rex::logging::tags::ENGINE_LOGGER_NAME).error("[INSTRUMENTOR] Instrumentor could not open results file '{0}'.", filepath);
        }
    }
    //--------------------------------------------------------------------------------------------
    void Instrumentor::end_session()
    {
        if (!m_enabled)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_mutex);

        internal_end_session();
    }

    //--------------------------------------------------------------------------------------------
    void Instrumentor::write_profile(const ProfileResult& result)
    {
        if (!m_enabled)
        {
            return;
        }

        std::stringstream json;

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        json << std::setprecision(3) << std::fixed;
        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << (result.elapsed_time.count()) << ',';
        json << "\"name\":\"" << name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":" << result.thread_id << ",";
        json << "\"ts\":" << result.start.count();
        json << "}";

        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_current_session)
        {
            m_output_stream << json.str();
            m_output_stream.flush();
        }
    }

    //--------------------------------------------------------------------------------------------
    Instrumentor& Instrumentor::get()
    {
        static Instrumentor instance;
        return instance;
    }

    //--------------------------------------------------------------------------------------------
    void Instrumentor::write_header()
    {
        m_output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
        m_output_stream.flush();
    }
    //--------------------------------------------------------------------------------------------
    void Instrumentor::write_footer()
    {
        m_output_stream << "]}";
        m_output_stream.flush();
    }

    //--------------------------------------------------------------------------------------------
    // Note: you must already own lock on m_mutex before calling InternalEndSession()
    void Instrumentor::internal_end_session()
    {
        if (m_current_session)
        {
            write_footer();
            m_output_stream.close();

            rex::logging::get_logger(rex::logging::tags::ENGINE_LOGGER_NAME).info("[INSTRUMENTOR] Ending session {0}", m_current_session->name);

            delete m_current_session;
            m_current_session = nullptr;
        }
    }

    //--------------------------------------------------------------------------------------------
    InstrumentationTimer::InstrumentationTimer(const char* name)
        : m_name(name)
        , m_stopped(false)
    {
        m_start_timepoint = std::chrono::steady_clock::now();
    }
    //--------------------------------------------------------------------------------------------
    InstrumentationTimer::~InstrumentationTimer()
    {
        if (!m_stopped)
        {
            stop();
        }
    }

    //--------------------------------------------------------------------------------------------
    void InstrumentationTimer::stop()
    {
        if (Instrumentor::get().is_enabled())
        {
            auto end_timepoint = std::chrono::steady_clock::now();
            auto highres_start = FloatingPointMicroseconds{m_start_timepoint.time_since_epoch()};
            auto elapsed_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch();

            Instrumentor::get().write_profile({m_name, highres_start, elapsed_time, std::this_thread::get_id()});
        }

        m_stopped = true;
    }
}
