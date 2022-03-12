#pragma once

#include "resources/texture_resource_manager_api.h"

namespace rex
{
    class Texture;
    
    class TextureResourceManager
    {
    public:
        static void intialize(std::unique_ptr<TextureResourceManagerAPI> manager);

        static uint32 max_combined_texture_image_units();

        static void activate_texture(const ref_ptr<rex::Texture>& texture, int32 location, IsRenderThread isRenderThread = IsRenderThread::NO);
        static void deactivate_texture(const ref_ptr<rex::Texture>& texture, IsRenderThread isRenderThread = IsRenderThread::NO);

        static void release_all();
    };
}