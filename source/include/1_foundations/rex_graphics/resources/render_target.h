#pragma once

#include "texture_2d.h"

namespace rex
{
    class RenderTarget : public Texture2D
    {
    public:
        virtual ~RenderTarget() = default;

        Texture::Type get_type() const override;
    };
}