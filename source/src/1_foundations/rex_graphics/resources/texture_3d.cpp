#include "rex_graphics_pch.h"

#include "resources/texture_3d.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Texture::Type Texture3D::get_type() const
    {
        return Texture::Type::TEXTURE_3D;
    }
}