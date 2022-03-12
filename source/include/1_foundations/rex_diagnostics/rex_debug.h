#pragma once

#if defined REX_DEBUG

//-------------------------------------------------------------------------
// Includes
#include "profiling/instrumentor.h"

#include "logging/logger.h"
#include "logging/loglevel.h"

#include <assert.h>
#include <stdio.h>

//-------------------------------------------------------------------------
// Assembly instruction to break execution.
#if HAR_DEBUG_BREAK
#if defined _MSC_VER
#define DEBUG_BREAK() __debugbreak()
#elif defined __clang__
#define DEBUG_BREAK() asm("int $3")
#elif defined __GNUC__
#define DEBUG_BREAK() __breakpoint(42)
#else
#error DEBUG_BREAK unsupported machine instruction ...
#endif
#else
#define DEBUG_BREAK()
#endif

//-------------------------------------------------------------------------
// REX Logger Names
#if defined(REX_CORE_LIB) || defined(REX_UTILITIES_LIB) || defined(REX_GLAD_LIB) || defined(REX_DIAGNOSTICS_LIB)
#define REX_LOGGER_NAME rex::logging::tags::ENGINE_LOGGER_NAME
#else
#define REX_LOGGER_NAME rex::logging::tags::CLIENT_LOGGER_NAME
#endif

//-------------------------------------------------------------------------
// Logging features
#define R_FATAL(...)                                                                                                                                                                                                                                     \
    rex::logging::get_logger(REX_LOGGER_NAME).critical(__VA_ARGS__);                                                                                                                                                                                     \
    DEBUG_BREAK()
#define R_ERROR(...) rex::logging::get_logger(REX_LOGGER_NAME).error(__VA_ARGS__)
#define R_WARN(...) rex::logging::get_logger(REX_LOGGER_NAME).warn(__VA_ARGS__)
#define R_INFO(...) rex::logging::get_logger(REX_LOGGER_NAME).info(__VA_ARGS__)
#define R_TODO(...) rex::logging::get_logger(REX_LOGGER_NAME).debug(__VA_ARGS__)
#define R_TRACE(...) rex::logging::get_logger(REX_LOGGER_NAME).trace(__VA_ARGS__)

//-------------------------------------------------------------------------
// Assertions
#ifdef REX_ASSERTIONS
#define R_ASSERT_X(expr, ...)                                                                                                                                                                                                                            \
    if (expr)                                                                                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                    \
    }                                                                                                                                                                                                                                                    \
    else                                                                                                                                                                                                                                                 \
    {                                                                                                                                                                                                                                                    \
        R_FATAL("Assertion Failed: {0}", __VA_ARGS__);                                                                                                                                                                                                   \
    }
#define R_ASSERT(expr)                                                                                                                                                                                                                                   \
    if (expr)                                                                                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                    \
    }                                                                                                                                                                                                                                                    \
    else                                                                                                                                                                                                                                                 \
    {                                                                                                                                                                                                                                                    \
        R_FATAL("Assertion Failed: {0}", #expr);                                                                                                                                                                                                         \
    }
#else
#define R_ASSERT_X(expr, ...)                                                                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                    \
        auto check = expr;                                                                                                                                                                                                                               \
        UNUSED_PARAM(check);                                                                                                                                                                                                                             \
    }
#define R_ASSERT(expr)
#endif

//-------------------------------------------------------------------------
// Verification
#define R_VERIFY_X(expr, ...) R_ASSERT_X(expr, __VA_ARGS__)
#define R_VERIFY(expr)                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
        auto check = expr;                                                                                                                                                                                                                               \
        UNUSED_PARAM(check);                                                                                                                                                                                                                             \
    }

//-------------------------------------------------------------------------
// Profiling
#define REX_ENABLE_PROFILE 0
#if REX_ENABLE_PROFILE

// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKR__) && (__MWERKR__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define R_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define R_FUNC_SIG __PRETTY_FUNCTION__
#elif(defined(__FUNCSIG__) || (_MSC_VER))
#define R_FUNC_SIG __FUNCSIG__
#elif(defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define R_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define R_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define R_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define R_FUNC_SIG __func__
#else
#define R_FUNC_SIG "HZ_FUNC_SIG unknown!"
#endif

#define _INTERNAL_R_PROFILE_SCOPE_LINE2(name, line)                                                                                                                                                                                                      \
    if (rex::Instrumentor::get().is_enabled())                                                                                                                                                                                                           \
    {                                                                                                                                                                                                                                                    \
        rex::InstrumentationTimer timer##line(name);                                                                                                                                                                                                     \
    }
#define _INTERNAL_R_PROFILE_SCOPE_LINE(name, line) _INTERNAL_R_PROFILE_SCOPE_LINE2(name, line)

#define R_PROFILE_BEGIN_SESSION(name, filepath) rex::Instrumentor::get().begin_session(name, filepath)
#define R_PROFILE_END_SESSION() rex::Instrumentor::get().end_session()
#define R_PROFILE_SCOPE(name) _INTERNAL_R_PROFILE_SCOPE_LINE(name, __LINE__)
#define R_PROFILE_FUNCTION() R_PROFILE_SCOPE(R_FUNC_SIG)
#define R_PROFILE_ENABLE() rex::Instrumentor::get().enable()
#define R_PROFILE_DISABLE() rex::Instrumentor::get().disable()
#else
#define R_PROFILE_BEGIN_SESSION(name, filepath)
#define R_PROFILE_END_SESSION()
#define R_PROFILE_SCOPE(name)
#define R_PROFILE_FUNCTION()
#define R_PROFILE_ENABLE()
#define R_PROFILE_DISABLE()
#endif

#else

#pragma warning(push)
#pragma warning(disable : 4702)

//
// Debugging features might be disabled but code from "spdlog" might still be used
// eg. fmt::ptr(...)
// Including certain spdlog header makes sure the code is still compiling.
//
#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>

#include <spdlog/fmt/ostr.h>

#pragma warning(pop)

//-------------------------------------------------------------------------
// Assembly instruction to break execution.
#define DEBUG_BREAK()

#define R_FATAL(...) UNUSED_PARAM(__VA_ARGS__)
#define R_ERROR(...) UNUSED_PARAM(__VA_ARGS__)
#define R_WARN(...) UNUSED_PARAM(__VA_ARGS__)
#define R_INFO(...) UNUSED_PARAM(__VA_ARGS__)
#define R_TODO(...) UNUSED_PARAM(__VA_ARGS__)
#define R_TRACE(...) UNUSED_PARAM(__VA_ARGS__)

#define R_ASSERT_X(expr, ...)                                                                                                                                                                                                                            \
    {                                                                                                                                                                                                                                                    \
        auto check = expr;                                                                                                                                                                                                                               \
        UNUSED_PARAM(check);                                                                                                                                                                                                                             \
    }
#define R_ASSERT(expr)

//-------------------------------------------------------------------------
// Verification
#define R_VERIFY_X(expr, ...) R_ASSERT_X(expr, __VA_ARGS__)
#define R_VERIFY(expr)                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
        auto check = expr;                                                                                                                                                                                                                               \
        UNUSED_PARAM(check);                                                                                                                                                                                                                             \
    }

#define R_INITIALIZE_LOGGER()
#define R_SHUTDOWN_LOGGER()

#define R_PROFILE_BEGIN_SESSION(name, filepath)
#define R_PROFILE_END_SESSION()
#define R_PROFILE_SCOPE(name)
#define R_PROFILE_FUNCTION()
#define R_PROFILE_ENABLE()
#define R_PROFILE_DISABLE()

#endif