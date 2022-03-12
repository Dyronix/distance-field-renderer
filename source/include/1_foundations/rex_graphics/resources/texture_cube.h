#pragma once

#include "resources/texture.h"

namespace rex
{
    struct TextureCubeDescription;

    class TextureCube : public Texture
    {
    public:
        Texture::Type get_type() const override;

        virtual void invalidate(TextureCubeDescription&& desc) = 0;
    };

    class SolidColorTextureCube
    {
    public:
        SolidColorTextureCube(rgba* textureData, uint32 size);

        void release();

        ref_ptr<TextureCube>& get_texture();
        const ref_ptr<TextureCube>& get_texture() const;

    private:
        ref_ptr<TextureCube> texture;
    };
}