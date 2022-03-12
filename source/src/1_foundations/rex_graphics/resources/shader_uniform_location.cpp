#include "rex_graphics_pch.h"

#include "resources/shader_uniform_location.h"

namespace rex
{
        //-------------------------------------------------------------------------
        ShaderUniformLocation ShaderUniformLocation::EMPTY = {0};

        //-------------------------------------------------------------------------
        ShaderUniformLocation::ShaderUniformLocation()
            : m_location(-1)
        {
        }
        //-------------------------------------------------------------------------
        ShaderUniformLocation::ShaderUniformLocation(int32 loc)
            : m_location(loc)
        {
        }

        //-------------------------------------------------------------------------
        ShaderUniformLocation::operator bool() const
        {
            return m_location != -1;
        }
        //-------------------------------------------------------------------------
        ShaderUniformLocation::operator int32() const
        {
            return m_location;
        }

        //-------------------------------------------------------------------------
        int32 ShaderUniformLocation::get() const
        {
            return m_location;
        }
}