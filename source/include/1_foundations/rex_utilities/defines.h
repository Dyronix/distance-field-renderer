#ifndef REX_DEFINES
#define REX_DEFINES

#if defined _MSC_VER
#define UNUSED_PARAM(...)                                                                                                                                                                                                                                \
    {                                                                                                                                                                                                                                                    \
        __VA_ARGS__;                                                                                                                                                                                                                                     \
    }
#elif defined __clang__
#define UNUSED_PARAM(...)                                                                                                                                                                                                                                \
    _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wunused-value\"")                                                                                                                                                              \
    {                                                                                                                                                                                                                                                    \
        __VA_ARGS__;                                                                                                                                                                                                                                     \
    }                                                                                                                                                                                                                                                    \
    _Pragma("clang diagnostic pop")
#elif defined __GNUC__
#define UNUSED_PARAM(...)                                                                                                                                                                                                                                \
    {                                                                                                                                                                                                                                                    \
        __VA_ARGS__;                                                                                                                                                                                                                                     \
    }
#endif

//-------------------------------------------------------------------------
// CPP VERSION
#if defined _MSC_VER
#if (_MSVC_LANG >= 201703L)
#define REX_HAS_CPP17 1
#else
#define REX_HAS_CPP17 0
#endif
#elif defined __clang__
#if (__cplusplus >= 201703L)
#define REX_HAS_CPP17 1
#else
#define REX_HAS_CPP17 0
#endif
#elif defined __GNUC__
#if (__cplusplus >= 201703L)
#define REX_HAS_CPP17 1
#else
#define REX_HAS_CPP17 0
#endif
#endif

//-------------------------------------------------------------------------
// DEBUGGING
#define REX_PROFILE 0

//-------------------------------------------------------------------------
// BIT TWIDDLING
#define BITS_IN_BYTE 8
#define BIT(x) (1 << x)

//-------------------------------------------------------------------------
// PLATFORM
#if defined REX_PLATFORM_WINDOWS
#if defined(_WIN64)
#define REX_PLATFORM_64BITS 1
#else
#define REX_PLATFORM_64BITS 0
#endif

// [[nodiscard]] attributes on STL functions
#ifndef REX_HAS_NODISCARD
#ifndef __has_cpp_attribute
#define REX_HAS_NODISCARD 0
#elif __has_cpp_attribute(nodiscard) >= 201603L  // TRANSITION, VSO#939899 (need toolset update)
#define REX_HAS_NODISCARD 1
#else
#define REX_HAS_NODISCARD 0
#endif
#endif  // REX_HAS_NODISCARD

#if REX_HAS_NODISCARD
#define REX_NODISCARD [[nodiscard]]
#else  // ^^^ CAN HAZ [[nodiscard]] / NO CAN HAZ [[nodiscard]] vvv
#define REX_NODISCARD
#endif  // REX_HAS_NODISCARD

#elif defined REX_PLATFORM_MACOS
#define REX_PLATFORM_64BITS 1
#elif defined REX_PLATFORM_LINUX
#if defined(_LINUX64) || defined(_LP64)
#define REX_PLATFORM_64BITS 1
#else
#define REX_PLATFORM_64BITS 0
#endif

// [[nodiscard]] attributes on STL functions
#ifndef REX_HAS_NODISCARD
#ifndef __has_cpp_attribute
#define REX_HAS_NODISCARD 0
#elif __has_cpp_attribute(nodiscard) >= 201603L  // TRANSITION, VSO#939899 (need toolset update)
#define REX_HAS_NODISCARD 1
#else
#define REX_HAS_NODISCARD 0
#endif
#endif  // REX_HAS_NODISCARD

#if REX_HAS_NODISCARD
#define REX_NODISCARD [[nodiscard]]
#else  // ^^^ CAN HAZ [[nodiscard]] / NO CAN HAZ [[nodiscard]] vvv
#define REX_NODISCARD
#endif  // REX_HAS_NODISCARD
#else
#error Unsupported platform
#endif

#endif  // REX_DEFINES