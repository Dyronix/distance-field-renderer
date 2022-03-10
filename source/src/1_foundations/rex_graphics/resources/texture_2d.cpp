#include "rex_graphics_pch.h"

#include "resources/texture_2d.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Texture::Type Texture2D::get_texture_type() const
    {
        return Texture::Type::TEXTURE_2D;
    }
}