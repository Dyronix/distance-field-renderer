#include "rex_windows_pch.h"

#include "graphics/resources/gl_texture_cube.h"
#include "graphics/resources/gl_texture_util.h"

#include "resources/texture_cube_description.h"

#include "renderer/renderer.h"

#include "pointer_math.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        std::unique_ptr<rex::opengl::TextureCube> TextureCube::create(TextureCubeDescription&& desc)
        {
            return std::make_unique<TextureCube>(std::move(desc));
        }

        //-------------------------------------------------------------------------
        TextureCube::TextureCube(const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_sampler_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_s(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_t(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_min_filter(Texture::Filter::Type::LINEAR)
            , m_mag_filter(Texture::Filter::Type::LINEAR)
            , m_format(Texture::Format::UNKNOWN)
            , m_texel_format(Texel::Format::UNKNOWN)
            , m_pixel_format(m_texel_format, m_format)
            , m_local_storage()
        {
            RENDERER_INFO("Submitting - Create Texture Cube: {0}", name.to_string());

            m_usage = textureUsage;
            m_texel_format = Texel::Format::RGBA;
            m_format = Texture::Format::RGBA_8;
            m_name = name;

            ref_ptr<TextureCube> instance(this);
            Renderer::submit([instance]() mutable
                             {
                                 RENDERER_INFO("Executing - Create Texture Cube: {0}", instance->m_name.to_string());

                                 opengl::generate_textures(1, &instance->m_id);

                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);

                                 instance->set_filter({Texture::Filter::Action::MINIFICATION, Texture::Filter::Type::LINEAR});
                                 instance->set_filter({Texture::Filter::Action::MAGNIFICATION, Texture::Filter::Type::LINEAR});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_S, Texture::Wrap::Type::REPEAT});
                                 instance->set_wrap({Texture::Wrap::Coordinate::WRAP_T, Texture::Wrap::Type::REPEAT});

                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                             });
        }
        //-------------------------------------------------------------------------
        TextureCube::TextureCube(uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_sampler_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_s(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_t(Texture::Wrap::Type::CLAMP_TO_EDGE)
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

            RENDERER_INFO("Submitting - Create Texture Cube: {0}", name.to_string());

            TextureCubeDescription description;

            description.width = width;
            description.height = height;
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
        TextureCube::TextureCube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_sampler_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_s(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_t(Texture::Wrap::Type::CLAMP_TO_EDGE)
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

            TextureCubeDescription description;

            description.width = width;
            description.height = height;
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
        TextureCube::TextureCube(TextureCubeDescription&& desc)
            : m_name(ESID::SID_None)
            , m_width(0)
            , m_height(0)
            , m_id(0)
            , m_sampler_id(0)
            , m_usage(Texture::Usage::UNSPECIFIED)
            , m_wrap_r(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_s(Texture::Wrap::Type::CLAMP_TO_EDGE)
            , m_wrap_t(Texture::Wrap::Type::CLAMP_TO_EDGE)
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
        TextureCube::~TextureCube()
        {
            release();
        }

        //-------------------------------------------------------------------------
        void TextureCube::invalidate()
        {
            invalidate(get_description(CopyImageData::YES));
        }
        //-------------------------------------------------------------------------
        void TextureCube::invalidate(TextureCubeDescription&& desc)
        {
            if (m_id)
            {
                release();
            }

            RENDERER_INFO("Submitting - Create Texture Cube: {0}", desc.name.to_string());

            m_width = (uint32)desc.width;
            m_height = (uint32)desc.height;

            m_usage = desc.usage;
            m_texel_format = desc.texel_format;
            m_format = desc.format;
            m_pixel_format = Pixel(m_texel_format, m_format);
            m_name = desc.name;

            store_data_chunks(std::move(desc.data));

            ref_ptr<TextureCube> instance(this);
            Renderer::submit([ instance, filters = desc.filters, wraps = desc.wraps ]() mutable
                             {
                                 RENDERER_INFO("Executing - Create Texture Cube: {0}", instance->m_name.to_string());

                                 opengl::generate_textures(1, &instance->m_id);
                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);

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
                                     for (int8 i = 0; i < default_wraps.size(); ++i)
                                     {
                                         instance->assign_wrap(default_wraps[i]);
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

                                 // auto target = GL_TEXTURE_CUBE_MAP;
                                 R_TODO("Target is not used, is this correct?");
                                 auto level = 0;
                                 auto internal_format = to_opengl_textureformat(instance->m_format);
                                 auto width = instance->m_width;
                                 auto height = instance->m_height;
                                 R_TODO("Depth is not used, is this correct?");
                                 // auto depth = NUM_CUBE_FACES;
                                 auto border = 0;
                                 auto format = to_opengl_texelformat(instance->m_texel_format);
                                 auto type = to_opengl_pixeltype(pixel.get_type());

                                 R_ASSERT(width != 0 && height != 0);

                                 uint32 offset = GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                                 for (uint8 i = 0; i < NUM_CUBE_FACES; ++i)
                                 {
                                     if (instance->m_local_storage[i] && instance->m_local_storage[i].get_size() > 0_bytes)
                                     {
                                         opengl::texture_image_2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (i * offset), level, internal_format, width, height, border, format, type, instance->m_local_storage[i].get_data_as<void>());
                                     }
                                     else
                                     {
                                         opengl::texture_image_2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (i * offset), level, internal_format, width, height, border, format, type, nullptr);
                                     }
                                 }

                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void TextureCube::release()
        {
            if (m_id)
            {
                opengl::delete_textures(1, &m_id);
                m_id = 0;
            }

            for (uint8 i = 0; i < NUM_CUBE_FACES; ++i)
            {
                m_local_storage[i].release();
            }
        }

        //-------------------------------------------------------------------------
        const rex::StringID& TextureCube::get_name() const
        {
            return m_name;
        }

        //-------------------------------------------------------------------------
        rex::TextureCubeDescription TextureCube::get_description(CopyImageData copyImageData /*= CopyImageData::NO*/) const
        {
            TextureCubeDescription description;

            description.width = m_width;
            description.height = m_height;

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
                size_t total_size = 0;
                for (int8 i = 0; i < NUM_CUBE_FACES; ++i)
                {
                    total_size += m_local_storage[i].get_size();
                }

                description.data.allocate(total_size);
                description.data.zero_initialize();

                size_t offset = 0;
                for (int8 i = 0; i < NUM_CUBE_FACES; ++i)
                {
                    auto ptr = pointer_math::jump_forward(description.data.get_data(), offset);

                    memory::Blob::copy(m_local_storage[i], ptr);

                    offset += m_local_storage[i].get_size();
                }
            }

            return description;
        }

        //-------------------------------------------------------------------------
        const rex::Texture::Data& TextureCube::get_data() const
        {
            R_TODO("Not sure if this actually valid ...");
            return m_local_storage[0];
        }

        //-------------------------------------------------------------------------
        rex::Texture::Usage TextureCube::get_usage() const
        {
            return m_usage;
        }
        //-------------------------------------------------------------------------
        rex::Texture::Format TextureCube::get_format() const
        {
            return m_format;
        }

        //-------------------------------------------------------------------------
        rex::Texel::Format TextureCube::get_channels() const
        {
            return m_texel_format;
        }

        //-------------------------------------------------------------------------
        uint32 TextureCube::get_width() const
        {
            return m_width;
        }
        //-------------------------------------------------------------------------
        uint32 TextureCube::get_height() const
        {
            return m_height;
        }

        //-------------------------------------------------------------------------
        uint32 TextureCube::get_id() const
        {
            return m_id;
        }

        //-------------------------------------------------------------------------
        void TextureCube::set_data(Texture::Data&& textureData)
        {
            R_ASSERT(m_width != 0 && m_height != 0);

            store_data_chunks(std::move(textureData));

            ref_ptr<TextureCube> instance(this);
            Renderer::submit([instance]()
                             {
                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);

                                 auto texelformat = to_opengl_texelformat(instance->m_texel_format);
                                 auto pixeltype = to_opengl_pixeltype(instance->m_pixel_format.get_type());

                                 uint32 offset = GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                                 for (uint8 i = 0; i < NUM_CUBE_FACES; ++i)
                                 {
                                     opengl::texture_sub_image_2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (i * offset), 0, 0, 0, instance->m_width, instance->m_height, texelformat, pixeltype, instance->m_local_storage[i].get_data_as<void>());
                                 }

                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void TextureCube::set_wrap(const Texture::Wrap& textureWrap)
        {
            ref_ptr<TextureCube> instance(this);
            Renderer::submit([instance, textureWrap]() mutable
                             {
                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);

                                 instance->assign_wrap(textureWrap);

                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                             });
        }

        //-------------------------------------------------------------------------
        void TextureCube::set_wrap_border_color(const ColorRGBA& inColor)
        {
            R_ASSERT(m_wrap_s == Texture::Wrap::Type::CLAMP_TO_BORDER || m_wrap_t == Texture::Wrap::Type::CLAMP_TO_BORDER || m_wrap_r == Texture::Wrap::Type::CLAMP_TO_BORDER);

            ref_ptr<TextureCube> instance(this);
            Renderer::submit(
                [instance, color = inColor]() mutable
                {
                    opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);

                    opengl::set_texture_float_array_parameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, color.get_data());

                    opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                });
        }

        //-------------------------------------------------------------------------
        void TextureCube::set_filter(const Texture::Filter& textureFilter)
        {
            ref_ptr<TextureCube> instance(this);
            Renderer::submit([instance, textureFilter]() mutable
                             {
                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);

                                 instance->assign_filter(textureFilter);

                                 opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                             });
        }
        //-------------------------------------------------------------------------
        void TextureCube::set_format(const Texel& texelFormat, const Texture::Format& textureFormat)
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
        void TextureCube::bind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_CUBE_MAP, m_id);
            }
            else
            {
                ref_ptr<const TextureCube> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_CUBE_MAP, instance->m_id);
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void TextureCube::unbind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
            }
            else
            {
                ref_ptr<const TextureCube> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::bind_texture(GL_TEXTURE_CUBE_MAP, 0);
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void TextureCube::assign_filter(const Filter& filter)
        {
            int32 type = filter.type == Texture::Filter::Type::NEAREST ? GL_NEAREST : GL_LINEAR;

            switch (filter.action)
            {
                case Texture::Filter::Action::MINIFICATION:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, type);
                    m_min_filter = filter.type;
                    break;
                case Texture::Filter::Action::MAGNIFICATION:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, type);
                    m_mag_filter = filter.type;
                    break;
            }
        }
        //-------------------------------------------------------------------------
        void TextureCube::assign_wrap(const Wrap& wrap)
        {
            int32 type = wrap.type == Texture::Wrap::Type::CLAMP_TO_EDGE 
                ? GL_CLAMP_TO_EDGE 
                : wrap.type == Texture::Wrap::Type::CLAMP_TO_BORDER
                    ? GL_CLAMP_TO_BORDER 
                    : GL_REPEAT;

            switch (wrap.coordinate)
            {
                case Texture::Wrap::Coordinate::WRAP_R:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, type);
                    m_wrap_r = wrap.type;
                    break;
                case Texture::Wrap::Coordinate::WRAP_S:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, type);
                    m_wrap_s = wrap.type;
                    break;
                case Texture::Wrap::Coordinate::WRAP_T:
                    opengl::set_texture_integer_parameter(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, type);
                    m_wrap_t = wrap.type;
                    break;
            }
        }

        //-------------------------------------------------------------------------
        void TextureCube::assign_wrap_color(const ColorRGBA& inColor)
        {
            R_ASSERT(m_wrap_s == Texture::Wrap::Type::CLAMP_TO_BORDER || m_wrap_t == Texture::Wrap::Type::CLAMP_TO_BORDER || m_wrap_r == Texture::Wrap::Type::CLAMP_TO_BORDER);

            ColorRGBA color = inColor;

            opengl::set_texture_float_array_parameter(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, color.get_data());
        }

        //-------------------------------------------------------------------------
        void TextureCube::store_data_chunks(Texture::Data&& data)
        {
            size_t size = data.get_size() / NUM_CUBE_FACES;
            size_t offset = 0u;

            for (uint32 i = 0; i < NUM_CUBE_FACES; ++i)
            {
                if (m_local_storage[i])
                {
                    m_local_storage[i].release();
                }

                auto ptr = pointer_math::jump_forward(data.get_data(), offset);

                m_local_storage[i] = memory::Blob::copy(ptr, size);

                offset += size;
            }
        }
    }
}