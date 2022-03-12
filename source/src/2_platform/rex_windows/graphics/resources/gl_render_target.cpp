#include "rex_windows_pch.h"

#include "renderer/renderer.h"

#include "graphics/resources/gl_render_target.h"
#include "graphics/resources/gl_texture_util.h"

#include "resources/texture_2d_description.h"

#define CHANNELS_RGBA 4
#define CHANNELS_RGB 3

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        ref_ptr<RenderTarget> RenderTarget::create(Texture2DDescription&& desc)
        {
            return make_ref<RenderTarget>(std::move(desc));
        }

        //-------------------------------------------------------------------------
        RenderTarget::RenderTarget(Texture2DDescription&& desc)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_sampler_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_s(Texture::Wrap::Type::CLAMP)
            , m_wrap_t(Texture::Wrap::Type::CLAMP)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (desc.width > GL_MAX_TEXTURE_SIZE || desc.height > GL_MAX_TEXTURE_SIZE)
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1} => max: {2} ", desc.width, desc.height, GL_MAX_TEXTURE_SIZE);
                return;
            }

            m_name = desc.name;
            m_width = (uint32)desc.width;
            m_height = (uint32)desc.height;

            m_usage = desc.usage;
            m_texel_format = desc.texel_format;
            m_format = desc.format;
            m_pixel_format = Pixel(m_texel_format, m_format);
            m_local_storage = memory::Blob::copy(desc.data.get_data(), desc.data.get_size());

            invalidate(std::move(desc));
        }
        //-------------------------------------------------------------------------
        RenderTarget::~RenderTarget()
        {
            release();
        }

        //-------------------------------------------------------------------------
        void RenderTarget::invalidate()
        {
            invalidate(get_description(CopyImageData::YES));
        }
        //-------------------------------------------------------------------------
        void RenderTarget::invalidate(Texture2DDescription&& desc)
        {
            if (m_id)
            {
                release();
            }

            opengl::generate_textures(1, &m_id);
            opengl::bind_texture(GL_TEXTURE_2D, m_id);

            auto pixel = Pixel(desc.texel_format, desc.format);

            auto target = GL_TEXTURE_2D;
            auto level = 0;
            auto internal_format = to_opengl_textureformat(desc.format);
            auto width = desc.width;
            auto height = desc.height;
            auto border = 0;
            auto format = to_opengl_texelformat(desc.texel_format);
            auto type = to_opengl_pixeltype(pixel.get_type());

            if (desc.data)
            {
                opengl::texture_image_2D(target, level, internal_format, width, height, border, format, type, desc.data.get_data_as<void>());
            }
            else
            {
                opengl::texture_image_2D(target, level, internal_format, width, height, border, format, type, nullptr);
            }

            if (desc.filters.size() == 0)
            {
                auto filters = default_texture_filter();
                for (auto& f : filters)
                {
                    set_filter(f);
                }
            }
            else
            {
                for (int8 i = 0; i < desc.filters.size(); ++i)
                {
                    set_filter(desc.filters[i]);
                }
            }

            if (desc.wraps.size() == 0)
            {
                for (int8 i = 0; i < desc.wraps.size(); ++i)
                {
                    set_wrap(desc.wraps[i]);
                }
            }
            else
            {
                auto wraps = default_texture_2D_wrapping();
                for (auto& w : wraps)
                {
                    set_wrap(w);
                }
            }

            opengl::bind_texture(GL_TEXTURE_2D, 0);
        }
        //-------------------------------------------------------------------------
        void RenderTarget::release()
        {
            opengl::delete_textures(1, &m_id);

            m_id = 0;
            m_local_storage.release();
        }

        //-------------------------------------------------------------------------
        const StringID& RenderTarget::get_name() const
        {
            return m_name;
        }

        //-------------------------------------------------------------------------
        const rex::Texture::Data& RenderTarget::get_data() const
        {
            return m_local_storage;
        }

        //-------------------------------------------------------------------------
        uint32 RenderTarget::get_width() const
        {
            return m_width;
        }
        //-------------------------------------------------------------------------
        uint32 RenderTarget::get_height() const
        {
            return m_height;
        }

        //-------------------------------------------------------------------------
        uint32 RenderTarget::get_id() const
        {
            return m_id;
        }

        //-------------------------------------------------------------------------
        Texture::Usage RenderTarget::get_usage() const
        {
            return m_usage;
        }
        //-------------------------------------------------------------------------
        Texture::Format RenderTarget::get_format() const
        {
            return m_format;
        }

        //-------------------------------------------------------------------------
        rex::Texel::Format RenderTarget::get_channels() const
        {
            return m_texel_format;
        }

        //-------------------------------------------------------------------------
        Texture2DDescription RenderTarget::get_description(CopyImageData copyImageData) const
        {
            Texture2DDescription description;

            description.width = m_width;
            description.height = m_height;

            description.texel_format = m_texel_format;

            description.usage = m_usage;
            description.format = m_format;

            description.wraps = std::vector<Texture::Wrap>(2);
            description.wraps[0].coordinate = Texture::Wrap::Coordinate::WRAP_S;
            description.wraps[0].type = m_wrap_s;
            description.wraps[1].coordinate = Texture::Wrap::Coordinate::WRAP_T;
            description.wraps[1].type = m_wrap_t;

            description.filters = std::vector<Texture::Filter>(2);
            description.filters[0].action = Texture::Filter::Action::MINIFICATION;
            description.filters[0].type = m_min_filter;
            description.filters[1].action = Texture::Filter::Action::MAGNIFICATION;
            description.filters[1].type = m_mag_filter;

            if (copyImageData)
            {
                description.data = memory::Blob::copy(m_local_storage.get_data(), m_local_storage.get_size());
            }

            return description;
        }

        //-------------------------------------------------------------------------
        void RenderTarget::set_data(Texture::Data&& textureData)
        {
            opengl::bind_texture(GL_TEXTURE_2D, m_id);

            auto texelformat = to_opengl_texelformat(m_texel_format);
            auto pixeltype = to_opengl_pixeltype(m_pixel_format.get_type());

            opengl::texture_sub_image_2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, texelformat, pixeltype, textureData.get_data_as<void>());
            opengl::bind_texture(GL_TEXTURE_2D, 0);
        }
        //-------------------------------------------------------------------------
        void RenderTarget::set_wrap(const Texture::Wrap& textureWrap)
        {
            opengl::bind_texture(GL_TEXTURE_2D, m_id);

            assign_wrap(textureWrap);

            opengl::bind_texture(GL_TEXTURE_2D, 0);
        }
        //-------------------------------------------------------------------------
        void RenderTarget::set_filter(const Texture::Filter& textureFilter)
        {
            opengl::bind_texture(GL_TEXTURE_2D, m_id);

            assign_filter(textureFilter);

            opengl::bind_texture(GL_TEXTURE_2D, 0);
        }
        //-------------------------------------------------------------------------
        void RenderTarget::set_format(const Texel& texelFormat, const Texture::Format& textureFormat)
        {
            if (m_id != 0)
            {
                R_ERROR("Changing the format of an already existing texture is not supported");
                return;
            }

            m_texel_format = texelFormat;
            m_format = textureFormat;
            m_pixel_format = Pixel(texelFormat, textureFormat);
        }

        //-------------------------------------------------------------------------
        void RenderTarget::bind(IsRenderThread rt /* = IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_2D, m_id);
            }
            else
            {
                ref_ptr<const RenderTarget> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void RenderTarget::unbind(IsRenderThread rt /* = IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_2D, 0);
            }
            else
            {
                ref_ptr<const RenderTarget> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_2D, 0);
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void RenderTarget::assign_filter(const Filter& filter)
        {
            int32 type = filter.type == Texture::Filter::Type::NEAREST ? GL_NEAREST : GL_LINEAR;

            switch (filter.action)
            {
                case Texture::Filter::Action::MINIFICATION:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
                    m_min_filter = filter.type;
                    break;
                case Texture::Filter::Action::MAGNIFICATION:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
                    m_mag_filter = filter.type;
                    break;
            }
        }
        //-------------------------------------------------------------------------
        void RenderTarget::assign_wrap(const Wrap& wrap)
        {
            int32 type = wrap.type == Texture::Wrap::Type::CLAMP ? GL_CLAMP_TO_EDGE : GL_REPEAT;

            switch (wrap.coordinate)
            {
                case Texture::Wrap::Coordinate::WRAP_R: R_WARN("This has no effect on a Texture2D"); break;

                case Texture::Wrap::Coordinate::WRAP_S:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
                    m_wrap_s = wrap.type;
                    break;
                case Texture::Wrap::Coordinate::WRAP_T:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);
                    m_wrap_t = wrap.type;
                    break;
            }
        }
    }
}

#undef CHANNELS_RGBA
#undef CHANNELS_RGB