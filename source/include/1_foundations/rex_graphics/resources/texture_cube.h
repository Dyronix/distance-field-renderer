#pragma once

#include "resources/texture.h"
#include "resources/texturecubedescription.h"

namespace sbt
{
    class TextureCube : public Texture
    {
    public:
        Texture::Type getTextureType() const override
        {
            return Texture::Type::TEXTURE_CUBE;
        }

        virtual void invalidate(TextureCubeDescription&& desc) = 0;
    };

    class SolidColorTextureCube
    {
    public:
        SolidColorTextureCube(rgba* textureData, uint32 size);

        void release();

        ref_ptr<TextureCube>& getTexture();
        const ref_ptr<TextureCube>& getTexture() const;

    private:
        ref_ptr<TextureCube> texture;
    };
}