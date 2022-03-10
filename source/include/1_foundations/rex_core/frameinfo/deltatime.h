#pragma once

#include <chrono>

namespace rex
{
    using fseconds = std::chrono::duration<float>;
    using fmiliseconds = std::chrono::duration<float, std::milli>;

    class DeltaTime
    {
    public:
        //-------------------------------------------------------------------------
        DeltaTime()
            : m_last_time_point(std::chrono::steady_clock::now())
            , m_current_time_point(std::chrono::steady_clock::now() - m_last_time_point)
        {
        }

        //-------------------------------------------------------------------------
        void update()
        {
            std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();

            m_current_time_point = fmiliseconds(current - m_last_time_point);
            m_last_time_point = current;
        }

        //--------------------------------------------------------------------------------------------
        float to_seconds() const
        {
#ifdef __EMSCRIPTEN__
            return 1.0f / 60.0f;
#else
            return fseconds{m_current_time_point}.count();
#endif
        }
        //--------------------------------------------------------------------------------------------
        float to_milliseconds() const
        {
#ifdef __EMSCRIPTEN__
            return (1.0f / 60.0f) * 1000;
#else
            return m_current_time_point.count();
#endif
        }

    private:
        std::chrono::steady_clock::time_point m_last_time_point;
        fmiliseconds m_current_time_point;
    };
}