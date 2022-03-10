#pragma once

#include "resources/render_target.h"

namespace rex
{
    namespace opengl
    {
        class RenderTarget : public rex::RenderTarget
        {
        public:
            using CopyImageData = YesNoEnum;

            static ref_ptr<RenderTarget> create(Texture2DDescription &&desc);

            RenderTarget(Texture2DDescription &&desc);
            ~RenderTarget() override;

            void invalidate() override;
            void invalidate(Texture2DDescription &&desc) override;
            void release() override;

            const StringID& get_name() const override;
            const Texture::Data& get_data() const override;

            uint32 get_width() const override;
            uint32 get_height() const override;

            uint32 get_id() const override;

            Texture::Usage get_usage() const override;
            Texture::Format get_format() const override;
            Texel::Format get_channels() const override;

            Texture2DDescription get_description(CopyImageData copyImageData = CopyImageData::NO) const;

            void set_data(Texture::Data &&textureData) override;
            void set_wrap(const Texture::Wrap &textureWrap) override;
            void set_filter(const Texture::Filter &textureFilter) override;
            void set_format(const Texel &texelFormat, const Texture::Format &textureFormat) override;

            void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;
            void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;

        private:
            void assign_filter(const Filter &filter);
            void assign_wrap(const Wrap &wrap);

            StringID m_name;

            uint32 m_width;
            uint32 m_height;
            uint32 m_id;
            uint32 m_sampler_id;

            Texture::Usage m_usage;
            Texture::Wrap::Type m_wrap_s;
            Texture::Wrap::Type m_wrap_t;
            Texture::Filter::Type m_min_filter;
            Texture::Filter::Type m_mag_filter;
            Texture::Format m_format;

            Texel m_texel_format;
            Pixel m_pixel_format;

            Texture::Data m_local_storage;
        };
    }
}