#pragma once

#include "resources/texture.h"
#include "resources/texture_2d_description.h"

namespace rex
{
    class Texture2D : public Texture
    {
    public:
        Texture::Type get_texture_type() const override;

        virtual void invalidate(Texture2DDescription&& desc) = 0;
    };
}