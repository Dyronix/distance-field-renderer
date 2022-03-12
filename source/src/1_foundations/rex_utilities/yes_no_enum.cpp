#include "rex_utilities_pch.h"

#include "yes_no_enum.h"

namespace rex
{
        //-------------------------------------------------------------------------
        YesNoEnum::YesNoEnum(Type value)
            : m_value(value)
        {
        }

        //-------------------------------------------------------------------------
        YesNoEnum::operator Type() const
        {
            return m_value;
        }

        //-------------------------------------------------------------------------
        YesNoEnum::operator bool() const
        {
            return m_value == Type::YES;
        }

        //-------------------------------------------------------------------------
        std::string YesNoEnum::to_string() const
        {
            if (m_value == Type::YES)
            {
                return "Yes";
            }

            return "No";
        }
}