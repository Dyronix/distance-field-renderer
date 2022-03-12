#pragma once

#include <chrono>

namespace rex
{
    //-------------------------------------------------------------------------
    // simulation of Windows GetTickCount()
    inline uint64 get_tick_count()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    //-------------------------------------------------------------------------
    // Clock built upon Windows GetTickCount()
    struct TickCountClock
    {
        typedef uint64 rep;
        typedef std::milli period;
        typedef std::chrono::duration<rep, period> duration;
        typedef std::chrono::time_point<TickCountClock> time_point;

        static const bool is_steady = true;

        //-------------------------------------------------------------------------
        static time_point now() noexcept
        {
            return time_point(duration(get_tick_count()));
        }
    };
}