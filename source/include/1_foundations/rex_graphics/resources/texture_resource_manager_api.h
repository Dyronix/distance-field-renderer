#pragma once

#include "resources/texture.h"

namespace sbt
{
    class TextureResourceManagerAPI
    {
    public:
        virtual ~TextureResourceManagerAPI() = default;

        virtual uint32 getMaxActivateTextureCount() const = 0;
        virtual uint32 getActivatedTextureCount() const = 0;

        virtual void activateTexture(const ref_ptr<sbt::Texture>& texture, int32 location, IsRenderThread isRenderThread = IsRenderThread::NO) = 0;
        virtual void deactivateTexture(const ref_ptr<sbt::Texture>& texture, IsRenderThread isRenderThread = IsRenderThread::NO) = 0;

        virtual void releaseAll() = 0;

    protected:
        TextureResourceManagerAPI() = default;
    };
}