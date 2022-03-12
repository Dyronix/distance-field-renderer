#include "rex_graphics_pch.h"

#include "resources/shader_resource_location.h"

namespace rex
{
        //-------------------------------------------------------------------------
        ShaderResourceLocation ShaderResourceLocation::EMPTY = {0};

        //-------------------------------------------------------------------------
        ShaderResourceLocation::ShaderResourceLocation()
            : m_location(-1)
        {
        }
        //-------------------------------------------------------------------------
        ShaderResourceLocation::ShaderResourceLocation(int32 loc)
            : m_location(loc)
        {
        }

        //-------------------------------------------------------------------------
        ShaderResourceLocation::operator bool() const
        {
            return m_location != -1;
        }
        //-------------------------------------------------------------------------
        ShaderResourceLocation::operator int32() const
        {
            return m_location;
        }

        //-------------------------------------------------------------------------
        int32 ShaderResourceLocation::get() const
        {
            return m_location;
        }
}