#pragma once

#include "logging/loglevel.h"

#pragma warning( push )
#pragma warning( disable : 4702 )

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>

#include <spdlog/fmt/ostr.h>

#pragma warning( pop )

namespace rex
{
    namespace logging
    {
        using LoggerObject = spdlog::logger;
        using LoggerObjectPtr = std::shared_ptr<LoggerObject>;
        using LoggerObjectPtrMap = std::unordered_map<std::string, LoggerObjectPtr>;

        using LoggerStream = std::ostringstream;
        using LoggerStreamsMap = std::unordered_map<std::string, LoggerStream>;

        struct level
        {
            static const LogLevel LOGGING;
        };

        struct tags
        {
            static const char* ENGINE_LOGGER_NAME;
            static const char* CLIENT_LOGGER_NAME;
        };

        void create();
        void destroy();

        bool addLogger(const char* name);
        bool addLogger(const char* name, const char* pattern);
        bool addLogger(const char* name, const char* pattern, rex::LogLevel level);

        bool hasLogger(const char* name);
        bool hasStream(const char* name);

        LoggerObject& getLogger(const char* name);
        LoggerStream& getStream(const char* name);
    }
}