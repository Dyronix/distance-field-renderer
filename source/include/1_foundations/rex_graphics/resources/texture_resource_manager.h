#pragma once

#include "resources/texture.h"
#include "resources/textureresourcemanagerapi.h"

namespace sbt
{
    class TextureResourceManager
    {
    public:
        static void intialize(std::unique_ptr<TextureResourceManagerAPI> manager);

        static uint32 maxCombinedTextureImageUnits();

        static void activateTexture(const ref_ptr<sbt::Texture>& texture, int32 location, IsRenderThread isRenderThread = IsRenderThread::NO);
        static void deactivateTexture(const ref_ptr<sbt::Texture>& texture, IsRenderThread isRenderThread = IsRenderThread::NO);

        static void releaseAll();
    };
}