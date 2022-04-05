#pragma once

#include "yes_no_enum.h"

#include "resources/texture_cube.h"

#define NUM_CUBE_FACES 6

namespace rex
{
    namespace opengl
    {
        class TextureCube : public rex::TextureCube
        {
        public:
            static std::unique_ptr<TextureCube> create(TextureCubeDescription&& desc);

            using CopyImageData = YesNoEnum;

            TextureCube(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
            TextureCube(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
            TextureCube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
            TextureCube(TextureCubeDescription&& desc);
            ~TextureCube() override;

            void invalidate() override;
            void invalidate(TextureCubeDescription&& desc) override;
            void release() override;

            const StringID& get_name() const override;

            TextureCubeDescription get_description(CopyImageData copyImageData = CopyImageData::NO) const;

            const Texture::Data& get_data() const override;
            Texture::Usage get_usage() const override;
            Texture::Format get_format() const override;
            Texel::Format get_channels() const override;

            uint32 get_width() const override;
            uint32 get_height() const override;

            uint32 get_id() const override;

            void set_data(Texture::Data&& textureData) override;
            void set_wrap(const Texture::Wrap& textureWrap) override;
            void set_wrap_border_color(const ColorRGBA& color) override;
            void set_filter(const Texture::Filter& textureFilter) override;
            void set_format(const Texel& texelFormat, const Texture::Format& textureFormat) override;

            void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;
            void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;

        private:
            void assign_filter(const Filter& filter);
            void assign_wrap(const Wrap& wrap);
            void assign_wrap_color(const ColorRGBA& color);
            void store_data_chunks(Texture::Data&& data);

            StringID m_name;

            uint32 m_width;
            uint32 m_height;
            uint32 m_id;
            uint32 m_sampler_id;

            Texture::Usage m_usage;
            Texture::Wrap::Type m_wrap_r;
            Texture::Wrap::Type m_wrap_s;
            Texture::Wrap::Type m_wrap_t;
            Texture::Filter::Type m_min_filter;
            Texture::Filter::Type m_mag_filter;
            Texture::Format m_format;

            Texel m_texel_format;
            Pixel m_pixel_format;

            Texture::Data m_local_storage[NUM_CUBE_FACES];
        };
    }
}