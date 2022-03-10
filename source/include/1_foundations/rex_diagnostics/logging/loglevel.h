#pragma once

#if defined REX_PLATFORM_WINDOWS
#undef ERROR  // Defined in wingdi.h
#endif

namespace rex
{
    enum class LogLevel
    {
        OFF,
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE
    };
}