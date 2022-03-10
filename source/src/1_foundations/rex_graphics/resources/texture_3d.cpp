#include "rex_graphics_pch.h"

#include "resources/texture_3d.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Texture::Type get_texture_type() const
    {
        return Texture::Type::TEXTURE_3D;
    }
}