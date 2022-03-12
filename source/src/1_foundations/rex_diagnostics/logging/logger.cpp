#include "rex_diagnostics_pch.h"

#include "logging/loglevel.h"
#include "logging/logger.h"

#include <assert.h>

#if defined REX_PLATFORM_WINDOWS
#undef ERROR  // Defined in wingdi.h
#endif

namespace rex
{
    namespace logging
    {
        using LogPattern = const char*;
        using LogLevelMap = std::unordered_map<rex::LogLevel, spdlog::level::level_enum>;

        const LogLevel level::LOGGING = rex::LogLevel::INFO;

        const char* tags::ENGINE_LOGGER_NAME = "REX";
        const char* tags::CLIENT_LOGGER_NAME = "APP";

        LoggerObjectPtrMap g_loggers;
        LoggerStreamsMap g_log_streams;

        //-------------------------------------------------------------------------
        LogLevelMap get_log_levels()
        {
            return {{rex::LogLevel::OFF, spdlog::level::off},    {rex::LogLevel::FATAL, spdlog::level::critical},
                    {rex::LogLevel::ERROR, spdlog::level::err},  {rex::LogLevel::WARN, spdlog::level::warn},
                    {rex::LogLevel::INFO, spdlog::level::info},  {rex::LogLevel::DEBUG, spdlog::level::debug},
                    {rex::LogLevel::TRACE, spdlog::level::trace}};
        }

        //-------------------------------------------------------------------------
        spdlog::logger* find_logger(const char* name)
        {
            auto it = g_loggers.find(name);

            return it != std::cend(g_loggers) ? (*it).second.get() : nullptr;
        }

        //-------------------------------------------------------------------------
        void create()
        {
            add_logger(tags::ENGINE_LOGGER_NAME);
            add_logger(tags::CLIENT_LOGGER_NAME);
        }
        //-------------------------------------------------------------------------
        void destroy()
        {
            g_loggers.clear();
        }

        //-------------------------------------------------------------------------
        bool add_logger(const char* name)
        {
            const LogPattern DEFAULT_PATTERN = ("%^[%T][%=8l] %n: %v%$");

            return add_logger(name, DEFAULT_PATTERN, level::LOGGING);
        }
        //-------------------------------------------------------------------------
        bool add_logger(const char* name, const char* pattern)
        {
            return add_logger(name, pattern, level::LOGGING);
        }
        //-------------------------------------------------------------------------
        bool add_logger(const char* name, const char* pattern, const rex::LogLevel& level)
        {
            const LogLevelMap LOG_LEVELS = get_log_levels();

            assert(LOG_LEVELS.find(level) != std::cend(LOG_LEVELS) && "Unknown log level was given");

            spdlog::logger* logger = find_logger(name);
            if (logger != nullptr)
                return false;

            g_log_streams.insert(std::make_pair(name, std::ostringstream()));  // create logger stream

            std::vector<spdlog::sink_ptr> sinks;
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            sinks.push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(g_log_streams.at(name)));

            std::shared_ptr<spdlog::logger> new_logger = std::make_shared<spdlog::logger>(name, std::begin(sinks), std::end(sinks));

            new_logger->set_pattern(pattern);
            new_logger->set_level(LOG_LEVELS.at(level));

            g_loggers.insert(std::make_pair(name, new_logger));

            return true;
        }

        //-------------------------------------------------------------------------
        bool has_logger(const char* name)
        {
            return find_logger(name) != nullptr;
        }
        //-------------------------------------------------------------------------
        bool has_stream(const char* name)
        {
            return g_log_streams.find(name) != std::cend(g_log_streams);
        }

        //-------------------------------------------------------------------------
        rex::logging::LoggerObject& get_logger(const char* name)
        {
            auto logger = find_logger(name);
            if (logger == nullptr)
            {
                add_logger(name);
                return get_logger(name);
            }

            return *logger;
        }
        //-------------------------------------------------------------------------
        rex::logging::LoggerStream& get_stream(const char* name)
        {
            auto logger = find_logger(name);
            if (logger == nullptr)
            {
                add_logger(name);
                return get_stream(name);
            }

            return g_log_streams.at(name);
        }
    }
}