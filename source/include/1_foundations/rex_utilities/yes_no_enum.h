#pragma once

#include <string>

namespace rex
{
    class YesNoEnum
    {
    public:
        enum Type
        {
            NO,
            YES
        };

        YesNoEnum(Type value);

        operator Type() const;
        operator bool() const;

        std::string to_string() const;

    private:
        Type m_value;
    };
}