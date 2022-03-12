#pragma once

namespace rex
{
    class API
    {
    public:
        enum class Type
        {
            UNSPECIFIED = BIT(0),

            OPENGL = BIT(1),
            OPENGL_ES = BIT(2)
        };

        API(Type value);

        Type get() const;

        bool equals(Type type) const;

        bool operator==(API other) const;
        bool operator!=(API other) const;

        operator int32() const;
        operator Type() const;
        operator bool() const;

        static API get_active();

        static std::array<API, 2> get_supported_api_types();

    private:
        Type m_value;
    };

    //-------------------------------------------------------------------------
    inline bool operator==(const API& api, const API::Type& apiType)
    {
        return api.get() == apiType;
    }
    //-------------------------------------------------------------------------
    inline bool operator!=(const API& api, const API::Type& apiType)
    {
        return api.get() != apiType;
    }

    //-------------------------------------------------------------------------
    inline bool operator==(const API::Type& apiType, const API& api)
    {
        return api.get() == apiType;
    }
    //-------------------------------------------------------------------------
    inline bool operator!=(const API::Type& apiType, const API& api)
    {
        return api.get() != apiType;
    }

    namespace conversions
    {
        const std::string to_string(API::Type type);
        const std::string to_display_string(API::Type type);
    }
}