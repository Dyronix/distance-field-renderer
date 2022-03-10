#pragma once

#include "logging/loglevel.h"

#pragma warning(push)
#pragma warning(disable : 4702)

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>

#include <spdlog/fmt/ostr.h>

#pragma warning(pop)

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

        bool add_logger(const char* name);
        bool add_logger(const char* name, const char* pattern);
        bool add_logger(const char* name, const char* pattern, rex::LogLevel level);

        bool has_logger(const char* name);
        bool has_stream(const char* name);

        LoggerObject& get_logger(const char* name);
        LoggerStream& get_stream(const char* name);
    }
}