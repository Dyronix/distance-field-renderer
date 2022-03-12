#pragma once

#include "resources/texture.h"

namespace rex
{
    struct Texture2DDescription;

    class Texture2D : public Texture
    {
    public:
        Texture::Type get_type() const override;

        virtual void invalidate(Texture2DDescription&& desc) = 0;
    };
}