#include "rex_windows_pch.h"

#include "renderer/renderer.h"

#include "graphics/resources/gl_texture_2d.h"
#include "graphics/resources/gl_texture_util.h"

#include "resources/texture_2d_description.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        std::unique_ptr<Texture2D> Texture2D::create(Texture2DDescription&& desc)
        {
            return std::make_unique<Texture2D>(std::move(desc));
        }

        //-------------------------------------------------------------------------
        Texture2D::Texture2D(const StringID& name, const Texture::Usage& textureUsage)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_s(Texture::Wrap::Type::REPEAT)
            , m_wrap_t(Texture::Wrap::Type::REPEAT)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            RENDERER_INFO("Submitting - Create Texture 2D: {0}", name.to_string());

            m_usage = textureUsage;
            m_texel_format = Texel::Format::RGBA;
            m_format = Texture::Format::RGBA_8;
            m_name = name;

            ref_ptr<Texture2D> instance(this);
            Renderer::submit([instance]() mutable
                             {
                                 RENDERER_INFO("Executing - Create Texture 2D: {0}", instance->m_name.to_string());

                                 opengl::generate_textures(1, &instance->m_id);

                                 opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);

                                 instance->set_filter({Texture::Filter::Action::MINIFICATION, Texture::Filter::Type::LINEAR});
                                 instance->set_filter({Texture::Filter::Action::MAGNIFICATION, Texture::Filter::Type::LINEAR});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_S, Texture::Wrap::Type::REPEAT});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_T, Texture::Wrap::Type::REPEAT});

                                 opengl::bind_texture(GL_TEXTURE_2D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        Texture2D::Texture2D(uint32 width, uint32 height, const StringID& name, const Texture::Usage& textureUsage)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_s(Texture::Wrap::Type::REPEAT)
            , m_wrap_t(Texture::Wrap::Type::REPEAT)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (gsl::narrow<int32>(width) > Renderer::get_max_texture_size() || gsl::narrow<int32>(height) > Renderer::get_max_texture_size())
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1} => max: {2} ", width, height, Renderer::get_max_texture_size());
                return;
            }

            RENDERER_INFO("Submitting - Create Texture 2D: {0}", name.to_string());

            Texture2DDescription description;

            description.width = width;
            description.height = height;
            description.name = name;
            description.texel_format = Texel::Format::RGBA;
            description.usage = textureUsage;
            description.format = Texture::Format::RGBA_8;
            description.wraps = Texture::default_texture_2D_wrapping();
            description.filters = Texture::default_texture_filter();
            description.data = nullptr;

            invalidate(std::move(description));
        }
        //-------------------------------------------------------------------------
        Texture2D::Texture2D(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name, const Texture::Usage& textureUsage)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_s(Texture::Wrap::Type::REPEAT)
            , m_wrap_t(Texture::Wrap::Type::REPEAT)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (gsl::narrow<int32>(width) > Renderer::get_max_texture_size() || gsl::narrow<int32>(height) > Renderer::get_max_texture_size())
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1} => max: {2} ", width, height, Renderer::get_max_texture_size());
                return;
            }

            Texture2DDescription description;

            description.width = width;
            description.height = height;
            description.name = name;
            description.texel_format = Texel::Format::RGBA;
            description.usage = textureUsage;
            description.format = Texture::Format::RGBA_8;
            description.wraps = Texture::default_texture_2D_wrapping();
            description.filters = Texture::default_texture_filter();
            description.data = std::move(textureData);

            invalidate(std::move(description));
        }
        //-------------------------------------------------------------------------
        Texture2D::Texture2D(Texture2DDescription&& desc)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_s(Texture::Wrap::Type::REPEAT)
            , m_wrap_t(Texture::Wrap::Type::REPEAT)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (gsl::narrow<int32>(desc.width) > Renderer::get_max_texture_size() || gsl::narrow<int32>(desc.height) > Renderer::get_max_texture_size())
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1} => max: {2} ", desc.width, desc.height, Renderer::get_max_texture_size());
                return;
            }

            invalidate(std::move(desc));
        }
        //-------------------------------------------------------------------------
        Texture2D::~Texture2D()
        {
            release();
        }

        //-------------------------------------------------------------------------
        void Texture2D::invalidate()
        {
            invalidate(get_description(CopyImageData::YES));
        }
        //-------------------------------------------------------------------------
        void Texture2D::invalidate(Texture2DDescription&& desc)
        {
            if (m_id)
            {
                release();
            }

            RENDERER_INFO("Submitting - Create Texture 2D: {0}", desc.name.to_string());

            m_width = (uint32)desc.width;
            m_height = (uint32)desc.height;

            m_usage = desc.usage;
            m_texel_format = desc.texel_format;
            m_format = desc.format;
            m_pixel_format = Pixel(m_texel_format, m_format);
            m_local_storage = memory::Blob::copy(desc.data.get_data(), desc.data.get_size());
            m_name = desc.name;

            ref_ptr<Texture2D> instance(this);
            Renderer::submit([ instance, filters = desc.filters, wraps = desc.wraps ]() mutable
                             {
                                 RENDERER_INFO("Executing - Create Texture 2D: {0}", instance->m_name.to_string());

                                 opengl::generate_textures(1, &instance->m_id);

                                 opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);

                                 if (filters.size() == 0)
                                 {
                                     auto default_filters = default_texture_filter();
                                     for (auto& f : default_filters)
                                     {
                                         instance->assign_filter(f);
                                     }
                                 }
                                 else
                                 {
                                     for (int8 i = 0; i < filters.size(); ++i)
                                     {
                                         instance->assign_filter(filters[i]);
                                     }
                                 }

                                 if (wraps.size() == 0)
                                 {
                                     auto default_wraps = default_texture_2D_wrapping();
                                     for (auto& w : default_wraps)
                                     {
                                         instance->assign_wrap(w);
                                     }
                                 }
                                 else
                                 {
                                     for (int8 i = 0; i < wraps.size(); ++i)
                                     {
                                         instance->assign_wrap(wraps[i]);
                                     }
                                 }

                                 auto pixel = Pixel(instance->m_texel_format, instance->m_format);

                                 auto target = GL_TEXTURE_2D;
                                 auto level = 0;
                                 auto internal_format = to_opengl_textureformat(instance->m_format);
                                 auto width = instance->m_width;
                                 auto height = instance->m_height;
                                 auto border = 0;
                                 auto format = to_opengl_texelformat(instance->m_texel_format);
                                 auto type = to_opengl_pixeltype(pixel.get_type());

                                 R_ASSERT(width != 0 && height != 0);

                                 if (instance->m_local_storage)
                                 {
                                     opengl::texture_image_2D(target, level, internal_format, width, height, border, format, type, instance->m_local_storage.get_data_as<void>());
                                 }
                                 else
                                 {
                                     opengl::texture_image_2D(target, level, internal_format, width, height, border, format, type, nullptr);
                                 }

                                 opengl::bind_texture(GL_TEXTURE_2D, 0);
                             });
        }

        //-------------------------------------------------------------------------
        void Texture2D::release()
        {
            if (m_id)
            {
                opengl::delete_textures(1, &m_id);
                m_id = 0;
            }

            m_local_storage.release();
        }

        //-------------------------------------------------------------------------
        const rex::StringID& Texture2D::get_name() const
        {
            return m_name;
        }

        //-------------------------------------------------------------------------
        Texture2DDescription Texture2D::get_description(CopyImageData copyImageData) const
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
        const rex::Texture::Data& Texture2D::get_data() const
        {
            return m_local_storage;
        }

        //-------------------------------------------------------------------------
        Texture::Usage Texture2D::get_usage() const
        {
            return m_usage;
        }
        //-------------------------------------------------------------------------
        Texture::Format Texture2D::get_format() const
        {
            return m_format;
        }

        //-------------------------------------------------------------------------
        rex::Texel::Format Texture2D::get_channels() const
        {
            return m_texel_format;
        }

        //-------------------------------------------------------------------------
        uint32 Texture2D::get_width() const
        {
            return m_width;
        }
        //-------------------------------------------------------------------------
        uint32 Texture2D::get_height() const
        {
            return m_height;
        }

        //-------------------------------------------------------------------------
        uint32 Texture2D::get_id() const
        {
            return m_id;
        }

        //-------------------------------------------------------------------------
        void Texture2D::set_data(Texture::Data&& textureData)
        {
            R_ASSERT(m_width != 0 && m_height != 0);

            ref_ptr<Texture2D> instance(this);
            Renderer::submit([ instance, data = std::move(textureData) ]()
                             {
                                 opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);

                                 auto texelformat = to_opengl_texelformat(instance->m_texel_format);
                                 auto pixeltype = to_opengl_pixeltype(instance->m_pixel_format.get_type());

                                 opengl::texture_sub_image_2D(GL_TEXTURE_2D, 0, 0, 0, instance->m_width, instance->m_height, texelformat, pixeltype, data.get_data_as<void>());

                                 opengl::bind_texture(GL_TEXTURE_2D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void Texture2D::set_wrap(const Texture::Wrap& textureWrap)
        {
            ref_ptr<Texture2D> instance(this);
            Renderer::submit([instance, textureWrap]() mutable
                             {
                                 opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);

                                 instance->assign_wrap(textureWrap);

                                 opengl::bind_texture(GL_TEXTURE_2D, 0);
                             });
        }

        //-------------------------------------------------------------------------
        void Texture2D::set_wrap_border_color(const ColorRGBA& inColor)
        {
            R_ASSERT(m_wrap_s == Texture::Wrap::Type::CLAMP_TO_BORDER || m_wrap_t == Texture::Wrap::Type::CLAMP_TO_BORDER);

            ref_ptr<Texture2D> instance(this);
            Renderer::submit([instance, color = inColor]() mutable
                {
                    opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);

                    opengl::set_texture_float_array_parameter(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color.get_data());

                    opengl::bind_texture(GL_TEXTURE_2D, 0);
                });
        }

        //-------------------------------------------------------------------------
        void Texture2D::set_filter(const Texture::Filter& textureFilter)
        {
            ref_ptr<Texture2D> instance(this);
            Renderer::submit([instance, textureFilter]() mutable
                             {
                                 opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);

                                 instance->assign_filter(textureFilter);

                                 opengl::bind_texture(GL_TEXTURE_2D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void Texture2D::set_format(const Texel& texelFormat, const Texture::Format& textureFormat)
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
        void Texture2D::bind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_2D, m_id);
            }
            else
            {
                ref_ptr<const Texture2D> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_2D, instance->m_id);
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void Texture2D::unbind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_2D, 0);
            }
            else
            {
                ref_ptr<const Texture2D> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_2D, 0);
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void Texture2D::assign_filter(const Filter& filter)
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
        void Texture2D::assign_wrap(const Wrap& wrap)
        {
            int32 type = wrap.type == Texture::Wrap::Type::CLAMP_TO_EDGE 
                ? GL_CLAMP_TO_EDGE 
                : wrap.type == Texture::Wrap::Type::CLAMP_TO_BORDER
                    ? GL_CLAMP_TO_BORDER 
                    : GL_REPEAT;

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

        //-------------------------------------------------------------------------
        void Texture2D::assign_wrap_color(const ColorRGBA& inColor)
        {
            R_ASSERT(m_wrap_s == Texture::Wrap::Type::CLAMP_TO_BORDER || m_wrap_t == Texture::Wrap::Type::CLAMP_TO_BORDER);

            ColorRGBA color = inColor;

            opengl::set_texture_float_array_parameter(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color.get_data());
        }

    } // namespace opengl
}