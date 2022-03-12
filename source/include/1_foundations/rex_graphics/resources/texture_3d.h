#pragma once

#include "resources/texture.h"

namespace rex
{
    struct Texture3DDescription;

    class Texture3D : public Texture
    {
    public:
        Texture::Type get_type() const override;

        virtual void invalidate(Texture3DDescription&& desc) = 0;

        virtual uint32 get_depth() const = 0;
    };
}