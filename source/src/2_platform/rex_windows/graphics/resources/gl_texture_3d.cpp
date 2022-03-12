#include "rex_windows_pch.h"

#include "renderer/renderer.h"

#include "graphics/resources/gl_texture_3d.h"
#include "graphics/resources/gl_texture_util.h"

#include "resources/texture_3d_description.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        std::unique_ptr<Texture3D> Texture3D::create(Texture3DDescription&& desc)
        {
            return std::make_unique<Texture3D>(std::move(desc));
        }

        //-------------------------------------------------------------------------
        Texture3D::Texture3D(const StringID& name, const Texture::Usage& textureUsage)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_depth(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP)
            , m_wrap_s(Texture::Wrap::Type::CLAMP)
            , m_wrap_t(Texture::Wrap::Type::CLAMP)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            RENDERER_INFO("Submitting - Create Texture 3D: name - {0}", name.to_string());

            m_usage = textureUsage;
            m_texel_format = Texel::Format::RGBA;
            m_format = Texture::Format::RGBA_8;
            m_name = name;

            ref_ptr<Texture3D> instance(this);
            Renderer::submit([instance]() mutable
                             {
                                 RENDERER_INFO("Executing - Create Texture 3D: name - {0}", "Unspecified");

                                 opengl::generate_textures(1, &instance->m_id);

                                 opengl::bind_texture(GL_TEXTURE_3D, instance->m_id);

                                 instance->set_filter({Texture::Filter::Action::MINIFICATION, Texture::Filter::Type::LINEAR});
                                 instance->set_filter({Texture::Filter::Action::MAGNIFICATION, Texture::Filter::Type::LINEAR});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_R, Texture::Wrap::Type::CLAMP});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_S, Texture::Wrap::Type::CLAMP});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_T, Texture::Wrap::Type::CLAMP});

                                 opengl::bind_texture(GL_TEXTURE_3D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        Texture3D::Texture3D(uint32 width, uint32 height, uint32 depth, const StringID& name, const Texture::Usage& textureUsage)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_depth(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP)
            , m_wrap_s(Texture::Wrap::Type::CLAMP)
            , m_wrap_t(Texture::Wrap::Type::CLAMP)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (width > GL_MAX_TEXTURE_SIZE || height > GL_MAX_TEXTURE_SIZE || depth > GL_MAX_TEXTURE_SIZE)
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1}, depth: {2} => max: {2} ", width, height, depth, GL_MAX_TEXTURE_SIZE);
                return;
            }

            RENDERER_INFO("Submitting - Create Texture 3D: {0}", name.to_string());

            Texture3DDescription description;

            description.width = width;
            description.height = height;
            description.depth = depth;
            description.name = name;
            description.texel_format = Texel::Format::RGBA;
            description.usage = textureUsage;
            description.format = Texture::Format::RGBA_8;
            description.wraps = Texture::default_texture_3D_wrapping();
            description.filters = Texture::default_texture_filter();
            description.data = nullptr;

            invalidate(std::move(description));
        }
        //-------------------------------------------------------------------------
        Texture3D::Texture3D(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth, const StringID& name, const Texture::Usage& textureUsage)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_depth(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP)
            , m_wrap_s(Texture::Wrap::Type::CLAMP)
            , m_wrap_t(Texture::Wrap::Type::CLAMP)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (width > GL_MAX_TEXTURE_SIZE || height > GL_MAX_TEXTURE_SIZE || depth > GL_MAX_TEXTURE_SIZE)
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1}, depth: {2} => max: {2} ", width, height, depth, GL_MAX_TEXTURE_SIZE);
                return;
            }

            Texture3DDescription description;

            description.width = width;
            description.height = height;
            description.depth = depth;
            description.name = name;
            description.texel_format = Texel::Format::RGBA;
            description.usage = textureUsage;
            description.format = Texture::Format::RGBA_8;
            description.wraps = Texture::default_texture_3D_wrapping();
            description.filters = Texture::default_texture_filter();
            description.data = std::move(textureData);

            invalidate(std::move(description));
        }
        //-------------------------------------------------------------------------
        Texture3D::Texture3D(Texture3DDescription&& desc)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_depth(0)
            , m_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP)
            , m_wrap_s(Texture::Wrap::Type::CLAMP)
            , m_wrap_t(Texture::Wrap::Type::CLAMP)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            if (desc.width > GL_MAX_TEXTURE_SIZE || desc.height > GL_MAX_TEXTURE_SIZE || desc.depth > GL_MAX_TEXTURE_SIZE)
            {
                R_ERROR("Exceeded max texture size: width: {0}, height: {1}, depth: {2} => max: {2} ", desc.width, desc.height, desc.depth, GL_MAX_TEXTURE_SIZE);
                return;
            }

            invalidate(std::move(desc));
        }
        //-------------------------------------------------------------------------
        Texture3D::~Texture3D()
        {
            release();
        }

        //-------------------------------------------------------------------------
        void Texture3D::invalidate()
        {
            invalidate(get_description(CopyImageData::YES));
        }
        //-------------------------------------------------------------------------
        void Texture3D::invalidate(Texture3DDescription&& desc)
        {
            if (m_id)
            {
                release();
            }

            RENDERER_INFO("Submitting - Create Texture 3D: {0}", desc.name.to_string());

            m_width = (uint32)desc.width;
            m_height = (uint32)desc.height;
            m_depth = (uint32)desc.depth;

            m_usage = desc.usage;
            m_texel_format = desc.texel_format;
            m_format = desc.format;
            m_pixel_format = Pixel(m_texel_format, m_format);
            m_local_storage = memory::Blob::copy(desc.data.get_data(), desc.data.get_size());
            m_name = desc.name;

            ref_ptr<Texture3D> instance(this);
            Renderer::submit([ instance, filters = desc.filters, wraps = desc.wraps ]() mutable
                             {
                                 RENDERER_INFO("Executing - Create Texture 3D: name - {0}", instance->m_name.to_string());

                                 opengl::generate_textures(1, &instance->m_id);

                                 opengl::bind_texture(GL_TEXTURE_3D, instance->m_id);

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
                                     auto default_wraps = default_texture_3D_wrapping();
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

                                 auto target = GL_TEXTURE_3D;
                                 auto level = 0;
                                 auto internal_format = to_opengl_textureformat(instance->m_format);
                                 auto width = instance->m_width;
                                 auto height = instance->m_height;
                                 auto depth = instance->m_depth;
                                 auto border = 0;
                                 auto format = to_opengl_texelformat(instance->m_texel_format);
                                 auto type = to_opengl_pixeltype(pixel.get_type());

                                 R_ASSERT(width != 0 && height != 0 && depth != 0);

                                 if (instance->m_local_storage)
                                 {
                                     opengl::texture_image_3D(target, level, internal_format, width, height, depth, border, format, type, instance->m_local_storage.get_data_as<void>());
                                 }
                                 else
                                 {
                                     opengl::texture_image_3D(target, level, internal_format, width, height, depth, border, format, type, nullptr);
                                 }

                                 opengl::bind_texture(GL_TEXTURE_3D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void Texture3D::release()
        {
            if (m_id)
            {
                opengl::delete_textures(1, &m_id);
                m_id = 0;
            }

            m_local_storage.release();
        }

        //-------------------------------------------------------------------------
        const rex::StringID& Texture3D::get_name() const
        {
            return m_name;
        }

        //-------------------------------------------------------------------------
        Texture3DDescription Texture3D::get_description(CopyImageData copyImageData) const
        {
            Texture3DDescription description;

            description.width = m_width;
            description.height = m_height;
            description.depth = m_depth;

            description.texel_format = m_texel_format;

            description.usage = m_usage;
            description.format = m_format;

            description.wraps = std::vector<Texture::Wrap>(3);
            description.wraps[0].coordinate = Texture::Wrap::Coordinate::WRAP_R;
            description.wraps[0].type = m_wrap_r;
            description.wraps[1].coordinate = Texture::Wrap::Coordinate::WRAP_S;
            description.wraps[1].type = m_wrap_s;
            description.wraps[2].coordinate = Texture::Wrap::Coordinate::WRAP_T;
            description.wraps[2].type = m_wrap_t;

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
        const rex::Texture::Data& Texture3D::get_data() const
        {
            return m_local_storage;
        }

        //-------------------------------------------------------------------------
        Texture::Usage Texture3D::get_usage() const
        {
            return m_usage;
        }
        //-------------------------------------------------------------------------
        Texture::Format Texture3D::get_format() const
        {
            return m_format;
        }

        //-------------------------------------------------------------------------
        rex::Texel::Format Texture3D::get_channels() const
        {
            return m_texel_format;
        }

        //-------------------------------------------------------------------------
        uint32 Texture3D::get_width() const
        {
            return m_width;
        }
        //-------------------------------------------------------------------------
        uint32 Texture3D::get_height() const
        {
            return m_height;
        }
        //-------------------------------------------------------------------------
        uint32 Texture3D::get_depth() const
        {
            return m_depth;
        }

        //-------------------------------------------------------------------------
        uint32 Texture3D::get_id() const
        {
            return m_id;
        }

        //-------------------------------------------------------------------------
        void Texture3D::set_data(Texture::Data&& textureData)
        {
            R_ASSERT(m_width != 0 && m_height != 0 && m_depth != 0);

            ref_ptr<Texture3D> instance(this);
            Renderer::submit([ instance, data = std::move(textureData) ]()
                             {
                                 opengl::bind_texture(GL_TEXTURE_3D, instance->m_id);

                                 auto texelformat = to_opengl_texelformat(instance->m_texel_format);
                                 auto pixeltype = to_opengl_pixeltype(instance->m_pixel_format.get_type());

                                 opengl::texture_sub_image_3D(GL_TEXTURE_3D, 0, 0, 0, 0, instance->m_width, instance->m_height, instance->m_depth, texelformat, pixeltype, data.get_data_as<int8_t>());

                                 opengl::bind_texture(GL_TEXTURE_3D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void Texture3D::set_wrap(const Texture::Wrap& textureWrap)
        {
            ref_ptr<Texture3D> instance(this);
            Renderer::submit([instance, textureWrap]() mutable
                             {
                                 opengl::bind_texture(GL_TEXTURE_3D, instance->m_id);

                                 instance->assign_wrap(textureWrap);

                                 opengl::bind_texture(GL_TEXTURE_3D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void Texture3D::set_filter(const Texture::Filter& textureFilter)
        {
            ref_ptr<Texture3D> instance(this);
            Renderer::submit([instance, textureFilter]() mutable
                             {
                                 opengl::bind_texture(GL_TEXTURE_3D, instance->m_id);

                                 instance->assign_filter(textureFilter);

                                 opengl::bind_texture(GL_TEXTURE_3D, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void Texture3D::set_format(const Texel& texelFormat, const Texture::Format& textureFormat)
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
        void Texture3D::bind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_3D, m_id);
            }
            else
            {
                ref_ptr<const Texture3D> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_3D, instance->m_id);
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void Texture3D::unbind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_3D, 0);
            }
            else
            {
                ref_ptr<const Texture3D> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_3D, 0);
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void Texture3D::assign_filter(const Filter& filter)
        {
            int32 type = filter.type == Texture::Filter::Type::NEAREST ? GL_NEAREST : GL_LINEAR;

            switch (filter.action)
            {
                case Texture::Filter::Action::MINIFICATION:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, type);
                    m_min_filter = filter.type;
                    break;
                case Texture::Filter::Action::MAGNIFICATION:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, type);
                    m_mag_filter = filter.type;
                    break;
            }
        }
        //-------------------------------------------------------------------------
        void Texture3D::assign_wrap(const Wrap& wrap)
        {
            int32 type = wrap.type == Texture::Wrap::Type::CLAMP ? GL_CLAMP_TO_EDGE : GL_REPEAT;

            switch (wrap.coordinate)
            {
                case Texture::Wrap::Coordinate::WRAP_R:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, type);
                    m_wrap_s = wrap.type;
                    break;
                case Texture::Wrap::Coordinate::WRAP_S:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, type);
                    m_wrap_s = wrap.type;
                    break;
                case Texture::Wrap::Coordinate::WRAP_T:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, type);
                    m_wrap_t = wrap.type;
                    break;
            }
        }
    }
}