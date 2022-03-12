#include "rex_utilities_pch.h"

#include "api_type.h"

namespace rex
{
    //-------------------------------------------------------------------------
    API::API(Type value)
        : m_value(value)
    {
    }

    //-------------------------------------------------------------------------
    API::Type API::get() const
    {
        return m_value;
    }

    //-------------------------------------------------------------------------
    bool API::equals(Type type) const
    {
        return m_value == type;
    }

    //-------------------------------------------------------------------------
    bool API::operator==(API other) const
    {
        return m_value == other.m_value;
    }
    //-------------------------------------------------------------------------
    bool API::operator!=(API other) const
    {
        return !(*this == other);
    }

    //-------------------------------------------------------------------------
    API::operator int32() const
    {
        return (int32)m_value;
    }
    //-------------------------------------------------------------------------
    API::operator Type() const
    {
        return m_value;
    }
    //-------------------------------------------------------------------------
    API::operator bool() const
    {
        return m_value != Type::UNSPECIFIED;
    }

    //-------------------------------------------------------------------------
    API API::get_active()

    {
#if defined REX_PLATFORM_WINDOWS
#if defined REX_API_OPENGL
        return API::Type::OPENGL;
#elif defined REX_API_OPENGLES
        return API::Type::OPENGL_ES;
#elif REX_API_NONE
        return API::Type::UNSPECIFIED;
#else
#error Unsupported API!
#endif
#elif defined REX_PLATFORM_MACOS
#if defined REX_API_OPENGL
        return API::Type::OPENGL;
#elif defined REX_API_OPENGLES
#error Unsupported API!
#elif REX_API_NONE
        return API::Type::UNSPECIFIED;
#else
#error Unsupported API!
#endif
#elif defined REX_PLATFORM_LINUX
#if defined REX_API_OPENGL
        return API::Type::OPENGL;
#elif defined REX_API_OPENGLES
#error Unsupported API!
#elif REX_API_NONE
        return API::Type::UNSPECIFIED;
#else
#error Unsupported API!
#endif
#elif defined REX_PLATFORM_EMSCRIPTEN
#if defined REX_API_OPENGL
#error Unsupported API!
#elif defined REX_API_OPENGLES
        return API::Type::OPENGL_ES;
#elif REX_API_NONE
        return API::Type::UNSPECIFIED;
#else
#error Unsupported API!
#endif
#else
#error Unsupported Platform!
#endif
    }
    //-------------------------------------------------------------------------
    std::array<API, 2> API::get_supported_api_types()
    {
        return {Type::OPENGL, Type::OPENGL_ES};
    }

    namespace conversions
    {
        //-------------------------------------------------------------------------
        const std::string to_string(API::Type type)
        {
            switch (type)
            {
                case API::Type::OPENGL: return "opengl";
                case API::Type::OPENGL_ES: return "opengles";
                case API::Type::UNSPECIFIED: return "unspecified";

                default: assert(false && "Unsupported API type"); return "";
            }
        }
        //-------------------------------------------------------------------------
        const std::string to_display_string(API::Type type)
        {
            switch (type)
            {
                case API::Type::OPENGL: return "Open GL";
                case API::Type::OPENGL_ES: return "Open GL ES";
                case API::Type::UNSPECIFIED: return "Unspecified";

                default: assert(false && "Unsupported API type"); return "";
            }
        }
    }
}