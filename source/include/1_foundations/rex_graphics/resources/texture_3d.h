#pragma once

#include "resources/texture.h"
#include "resources/texture_3d_description.h"

namespace rex
{
    class Texture3D : public Texture
    {
    public:
        Texture::Type get_texture_type() const override;

        virtual void invalidate(Texture3DDescription&& desc) = 0;
    };
}