#pragma once

#include "resources/texture_resource_manager_api.h"

namespace rex
{
    namespace opengl
    {
        class TextureResourceManagerAPI : public rex::TextureResourceManagerAPI
        {
        public:
            TextureResourceManagerAPI(uint32 maxTextureCount, uint32 maxTextureSize);
            ~TextureResourceManagerAPI() override;

            const int32 get_activated_texture_count() const override;
            const int32 get_max_texture_size() const override;
            const int32 get_max_activate_texture_count() const override;

            void activate_texture(const ref_ptr<rex::Texture>& texture, int32 location, IsRenderThread isRenderThread = IsRenderThread::NO) override;
            void deactivate_texture(const ref_ptr<rex::Texture>& texture, IsRenderThread isRenderThread = IsRenderThread::NO) override;

            void release_all() override;
        };
    }
}