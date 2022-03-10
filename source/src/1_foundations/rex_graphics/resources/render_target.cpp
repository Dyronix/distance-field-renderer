#include "rex_graphics_pch.h"

#include "resources/render_target.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Texture::Type get_texture_type() const
    {
        return Texture::Type::RENDER_TARGET;
    }
}