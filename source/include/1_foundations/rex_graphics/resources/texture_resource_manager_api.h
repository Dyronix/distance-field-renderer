#pragma once

namespace rex
{
    class Texture;
    
    class TextureResourceManagerAPI
    {
    public:
        virtual ~TextureResourceManagerAPI() = default;

        virtual uint32 get_activated_texture_count() const = 0;
        virtual uint32 get_max_activate_texture_count() const = 0;

        virtual void activate_texture(const ref_ptr<rex::Texture>& texture, int32 location, IsRenderThread isRenderThread = IsRenderThread::NO) = 0;
        virtual void deactivate_texture(const ref_ptr<rex::Texture>& texture, IsRenderThread isRenderThread = IsRenderThread::NO) = 0;

        virtual void release_all() = 0;

    protected:
        TextureResourceManagerAPI() = default;
    };
}