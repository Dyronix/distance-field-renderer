#include "rex_graphics_pch.h"

#include "resources/texture.h"
#include "resources/texture_2d_description.h"
#include "resources/resource_factory.h"

#include "blobwriter.h"

namespace rex
{
    //-------------------------------------------------------------------------
    const std::vector<rex::Texture::Wrap> g_default_2D_texture_wrap = {
        {rex::Texture::Wrap::Coordinate::WRAP_S, rex::Texture::Wrap::Type::REPEAT}, {rex::Texture::Wrap::Coordinate::WRAP_T, rex::Texture::Wrap::Type::REPEAT},
    };

    const std::vector<rex::Texture::Wrap> g_default_3D_texture_wrap = {
        {rex::Texture::Wrap::Coordinate::WRAP_R, rex::Texture::Wrap::Type::CLAMP}, {rex::Texture::Wrap::Coordinate::WRAP_S, rex::Texture::Wrap::Type::CLAMP}, {rex::Texture::Wrap::Coordinate::WRAP_T, rex::Texture::Wrap::Type::CLAMP},
    };

    const std::vector<rex::Texture::Filter> g_default_texture_filter = {
        {rex::Texture::Filter::Action::MINIFICATION, rex::Texture::Filter::Type::LINEAR}, {rex::Texture::Filter::Action::MAGNIFICATION, rex::Texture::Filter::Type::LINEAR},
    };

    //-------------------------------------------------------------------------
    Pixel get_pixel_format(const Texel::Format& texelFormat, const Texture::Format& textureFormat)
    {
        static std::unordered_map<Texel::Format, std::unordered_map<Texture::Format, Pixel>> pixel_transfers = {
            {Texel::Format::R,
             {{Texture::Format::R_8, Pixel(Pixel::Format::R, Pixel::Type::UNSIGNED_BYTE)},
              {Texture::Format::R_8_INTEGER, Pixel(Pixel::Format::R_INTEGER, Pixel::Type::INT)},
              {Texture::Format::R_8_UNSIGNED_INTEGER, Pixel(Pixel::Format::R_INTEGER, Pixel::Type::UNSIGNED_INT)},

              {Texture::Format::R_16, Pixel(Pixel::Format::R, Pixel::Type::UNSIGNED_SHORT)},
              {Texture::Format::R_16_INTEGER, Pixel(Pixel::Format::R_INTEGER, Pixel::Type::INT)},
              {Texture::Format::R_16_UNSIGNED_INTEGER, Pixel(Pixel::Format::R_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::R_16_FLOAT, Pixel(Pixel::Format::R, Pixel::Type::HALF_FLOAT)},

              {Texture::Format::R_32_INTEGER, Pixel(Pixel::Format::R_INTEGER, Pixel::Type::INT)},
              {Texture::Format::R_32_UNSIGNED_INTEGER, Pixel(Pixel::Format::R_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::R_32_FLOAT, Pixel(Pixel::Format::R, Pixel::Type::FLOAT)}}},

            {Texel::Format::RG,
             {{Texture::Format::RG_8, Pixel(Pixel::Format::RG, Pixel::Type::UNSIGNED_BYTE)},
              {Texture::Format::RG_8_INTEGER, Pixel(Pixel::Format::RG_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RG_8_UNSIGNED_INTEGER, Pixel(Pixel::Format::RG_INTEGER, Pixel::Type::UNSIGNED_INT)},

              {Texture::Format::RG_16, Pixel(Pixel::Format::RG, Pixel::Type::UNSIGNED_SHORT)},
              {Texture::Format::RG_16_INTEGER, Pixel(Pixel::Format::RG_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RG_16_UNSIGNED_INTEGER, Pixel(Pixel::Format::RG_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::RG_16_FLOAT, Pixel(Pixel::Format::RG, Pixel::Type::HALF_FLOAT)},

              {Texture::Format::RG_32_INTEGER, Pixel(Pixel::Format::RG_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RG_32_UNSIGNED_INTEGER, Pixel(Pixel::Format::RG_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::RG_32_FLOAT, Pixel(Pixel::Format::RG, Pixel::Type::FLOAT)}}},

            {Texel::Format::RGB,
             {{Texture::Format::RGB_8, Pixel(Pixel::Format::RGB, Pixel::Type::UNSIGNED_BYTE)},
              {Texture::Format::RGB_8_INTEGER, Pixel(Pixel::Format::RGB_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RGB_8_UNSIGNED_INTEGER, Pixel(Pixel::Format::RGB_INTEGER, Pixel::Type::UNSIGNED_INT)},

              {Texture::Format::RGB_16, Pixel(Pixel::Format::RGB, Pixel::Type::UNSIGNED_SHORT)},
              {Texture::Format::RGB_16_INTEGER, Pixel(Pixel::Format::RGB_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RGB_16_UNSIGNED_INTEGER, Pixel(Pixel::Format::RGB_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::RGB_16_FLOAT, Pixel(Pixel::Format::RGB, Pixel::Type::HALF_FLOAT)},

              {Texture::Format::RGB_32_INTEGER, Pixel(Pixel::Format::RGB_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RGB_32_UNSIGNED_INTEGER, Pixel(Pixel::Format::RGB_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::RGB_32_FLOAT, Pixel(Pixel::Format::RGB, Pixel::Type::FLOAT)}}},

            {Texel::Format::RGBA,
             {{Texture::Format::RGBA_8, Pixel(Pixel::Format::RGBA, Pixel::Type::UNSIGNED_BYTE)},
              {Texture::Format::RGBA_8_INTEGER, Pixel(Pixel::Format::RGBA_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RGBA_8_UNSIGNED_INTEGER, Pixel(Pixel::Format::RGBA_INTEGER, Pixel::Type::UNSIGNED_INT)},

              {Texture::Format::RGBA_16, Pixel(Pixel::Format::RGBA, Pixel::Type::UNSIGNED_SHORT)},
              {Texture::Format::RGBA_16_INTEGER, Pixel(Pixel::Format::RGBA_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RGBA_16_UNSIGNED_INTEGER, Pixel(Pixel::Format::RGBA_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::RGBA_16_FLOAT, Pixel(Pixel::Format::RGBA, Pixel::Type::HALF_FLOAT)},

              {Texture::Format::RGBA_32_INTEGER, Pixel(Pixel::Format::RGBA_INTEGER, Pixel::Type::INT)},
              {Texture::Format::RGBA_32_UNSIGNED_INTEGER, Pixel(Pixel::Format::RGBA_INTEGER, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::RGBA_32_FLOAT, Pixel(Pixel::Format::RGBA, Pixel::Type::FLOAT)}}},

            {Texel::Format::DEPTH_COMPONENT,
             {{Texture::Format::DEPTH_COMPONENT_16_INTEGER, Pixel(Pixel::Format::DEPTH_COMPONENT, Pixel::Type::UNSIGNED_SHORT)},
              {Texture::Format::DEPTH_COMPONENT_24_INTEGER, Pixel(Pixel::Format::DEPTH_COMPONENT, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::DEPTH_COMPONENT_32_INTEGER, Pixel(Pixel::Format::DEPTH_COMPONENT, Pixel::Type::UNSIGNED_INT)},
              {Texture::Format::DEPTH_COMPONENT_32_FLOAT, Pixel(Pixel::Format::DEPTH_COMPONENT, Pixel::Type::FLOAT)}}},

            {Texel::Format::DEPTH_STENCIL_COMPONENT,
             {{Texture::Format::DEPTH_COMPONENT_24_UNSIGNED_INTEGER_STENCIL_8_UNSIGNED_INTEGER, Pixel(Pixel::Format::DEPTH_STENCIL, Pixel::Type::UNSIGNED_INT_24_8)},
              {Texture::Format::DEPTH_COMPONENT_32_FLOAT_STENCIL_8_FLOAT, Pixel(Pixel::Format::DEPTH_STENCIL, Pixel::Type::FLOAT_32_UNSIGNED_INT_24_8_REV)}}}};

        auto texel_it = pixel_transfers.find(texelFormat);
        if (texel_it == std::cend(pixel_transfers))
        {
            return Pixel();
        }

        auto texture_it = texel_it->second.find(textureFormat);
        if (texture_it == std::cend(texel_it->second))
        {
            return Pixel();
        }

        return texture_it->second;
    }

    //-------------------------------------------------------------------------
    Pixel::Pixel()
        : m_format(Pixel::Format::UNKNOWN)
        , m_type(Pixel::Type::UNKNOWN)
    {
    }
    //-------------------------------------------------------------------------
    Pixel::Pixel(const Texel::Format& texelFormat, const Texture::Format& textureFormat)
        : Pixel(get_pixel_format(texelFormat, textureFormat))
    {
    }
    //-------------------------------------------------------------------------
    Pixel::Pixel(const Format& pixelFormat, const Type& pixelType)
        : m_type(pixelType)
        , m_format(pixelFormat)
    {
    }
    //-------------------------------------------------------------------------
    Pixel::Pixel(const Pixel& other)
        : m_format(other.m_format)
        , m_type(other.m_type)
    {
    }

    //-------------------------------------------------------------------------
    Pixel::~Pixel()
    {
    }

    //-------------------------------------------------------------------------
    const rex::Pixel::Format Pixel::get_format() const
    {
        return m_format;
    }
    //-------------------------------------------------------------------------
    const rex::Pixel::Type Pixel::get_type() const
    {
        return m_type;
    }

    //-------------------------------------------------------------------------
    uint32 Texel::get_channel_count(const Texel::Format& format)
    {
        Texel t(format);

        return t.get_count();
    }

    //-------------------------------------------------------------------------
    Texel::Texel()
        : m_format(Texel::Format::UNKNOWN)
    {
    }
    //-------------------------------------------------------------------------
    Texel::Texel(const Texel::Format& format)
        : m_format(format)
    {
    }
    //-------------------------------------------------------------------------
    Texel::~Texel()
    {
    }

    //-------------------------------------------------------------------------
    uint32 Texel::get_count() const
    {
        switch (m_format)
        {
            case Texel::Format::R: return 1u;
            case Texel::Format::RG: return 2u;
            case Texel::Format::RGB: return 3u;
            case Texel::Format::RGBA: return 4u;

            case Texel::Format::DEPTH_COMPONENT: R_WARN("No channels specified for the depth component format"); return 0u;
            case Texel::Format::DEPTH_STENCIL_COMPONENT: R_WARN("No channels specified for the depth stencil component format"); return 0u;

            default: R_ASSERT_X(false, "Unsupported Texel Format"); return 0u;
        }
    }

    //-------------------------------------------------------------------------
    Texel::operator uint32() const
    {
        return static_cast<uint32>(m_format);
    }
    //-------------------------------------------------------------------------
    Texel::operator Texel::Format() const
    {
        return m_format;
    }

    //-------------------------------------------------------------------------
    std::string Texel::to_string() const
    {
        switch (m_format)
        {
            case Texel::Format::R: return "Red";
            case Texel::Format::RG: return "Red Green";
            case Texel::Format::RGB: return "Red Green Blue";
            case Texel::Format::RGBA: return "Red Green Blue Alpha";

            case Texel::Format::DEPTH_COMPONENT: return "Depth";
            case Texel::Format::DEPTH_STENCIL_COMPONENT: return "Depth Stencil";

            default: R_ASSERT_X(false, "Unsupported image format"); return "";
        }
    }

    //-------------------------------------------------------------------------
    SolidColorTexture::SolidColorTexture(rgba textureData)
    {
        const uint32 data_width = 1u;
        const uint32 data_height = 1u;

        Texture2DDescription description;

        description.width = data_width;
        description.height = data_height;
        description.format = Texture::Format::RGBA_8;
        description.texel_format = Texel::Format::RGBA;
        description.name = "Solid Color Texture";
        description.usage = Texture::Usage::UNSPECIFIED;

        description.data.release();
        description.data.allocate(data_width * data_height * description.texel_format.get_count());
        description.data.zero_initialize();

        memory::writer::write(description.data, (void*)&textureData, data_width * data_height * description.texel_format.get_count());

        description.filters = {{Texture::Filter::Action::MINIFICATION, Texture::Filter::Type::LINEAR}, {Texture::Filter::Action::MAGNIFICATION, Texture::Filter::Type::LINEAR}};
        description.wraps = {
            {Texture::Wrap::Coordinate::WRAP_S, Texture::Wrap::Type::REPEAT}, {Texture::Wrap::Coordinate::WRAP_T, Texture::Wrap::Type::REPEAT},
        };

        texture = ResourceFactory::create_texture2d(std::move(description));
    }

    //-------------------------------------------------------------------------
    void SolidColorTexture::release()
    {
        texture->release();
        texture.reset();
    }

    //-------------------------------------------------------------------------
    ref_ptr<Texture>& SolidColorTexture::get_texture()
    {
        return texture;
    }
    //-------------------------------------------------------------------------
    const ref_ptr<Texture>& SolidColorTexture::get_texture() const
    {
        return texture;
    }

    namespace conversions
    {
        //-------------------------------------------------------------------------
        std::string to_display_string(const Texture::Type& type)
        {
            switch (type)
            {
                case Texture::Type::TEXTURE_2D: return "Texture Type: 2D";
                case Texture::Type::TEXTURE_3D: return "Texture Type: 3D";
                case Texture::Type::TEXTURE_CUBE: return "Texture Type: CUBE";
                case Texture::Type::RENDER_TARGET: return "Texture Type: RENDER TARGET";
            }

            R_ASSERT_X(false, "Unreachable code");
            return "";
        }
        //-------------------------------------------------------------------------
        std::string to_display_string(const Texture::Usage& usage)
        {
            switch (usage)
            {
                case Texture::Usage::UNSPECIFIED: return "Texture Usage: Unspecified";

                case Texture::Usage::DIFFUSE: return "Texture Usage: Diffuse";
                case Texture::Usage::SPECUALR: return "Texture Usage: Specular";
                case Texture::Usage::NORMAL: return "Texture Usage: Normal";

                case Texture::Usage::COLOR_ATTACHMENT: return "Texture Usage: Color Attachment";
                case Texture::Usage::DEPTH_ATTACHMENT: return "Texture Usage: Depth Attachment";

                case Texture::Usage::CUBEMAP_ATTACHMENT: return "Texture Usage: CubeMap Attachment";
            }

            R_ASSERT_X(false, "Unreachable code");
            return "";
        }
        //-------------------------------------------------------------------------
        std::string to_display_string(const Texture::Format& format)
        {
            switch (format)
            {
                case Texture::Format::UNKNOWN: return "Texture Format: Unknown";

                case Texture::Format::R_8: return "Texture Format: Red - 8 bit - unsigned byte";
                case Texture::Format::R_8_INTEGER: return "Texture Format: Red - 8 bit - byte";
                case Texture::Format::R_8_UNSIGNED_INTEGER: return "Texture Format: Red - 8 bit - unsigned byte";

                case Texture::Format::R_16: return "Texture Format: Red - 16 bit - unsigned char";
                case Texture::Format::R_16_INTEGER: return "Texture Format: Red - 16 bit - short";
                case Texture::Format::R_16_UNSIGNED_INTEGER: return "Texture Format: Red - 16 bit - unsigned short";
                case Texture::Format::R_16_FLOAT: return "Texture Format: Red - 16 bit - float";

                case Texture::Format::R_32_INTEGER: return "Texture Format: Red - 32 bit - integer";
                case Texture::Format::R_32_UNSIGNED_INTEGER: return "Texture Format: Red - 32 bit - unsigned integer";
                case Texture::Format::R_32_FLOAT: return "Texture Format: Red - 32 bit - float";

                case Texture::Format::RG_8: return "Texture Format: Red Green - 8 bit - unsigned byte";
                case Texture::Format::RG_8_INTEGER: return "Texture Format: Red Green - 8 bit - byte";
                case Texture::Format::RG_8_UNSIGNED_INTEGER: return "Texture Format: Red Green - 8 bit - unsigned byte";

                case Texture::Format::RG_16: return "Texture Format: Red Green - 16 bit - unsigned byte";
                case Texture::Format::RG_16_INTEGER: return "Texture Format: Red Green - 16 bit - short";
                case Texture::Format::RG_16_UNSIGNED_INTEGER: return "Texture Format: Red Green - 16 bit - unsigned short";
                case Texture::Format::RG_16_FLOAT: return "Texture Format: Red Green - 16 bit - float";

                case Texture::Format::RG_32_INTEGER: return "Texture Format: Red Green - 32 bit - integer";
                case Texture::Format::RG_32_UNSIGNED_INTEGER: return "Texture Format:  Red Green - 32 bit - unsigned integer";
                case Texture::Format::RG_32_FLOAT: return "Texture Format:  Red Green - 32 bit - float";

                case Texture::Format::RGB_8: return "Texture Format: Red Green Blue - 8 bit - unsigned byte";
                case Texture::Format::RGB_8_INTEGER: return "Texture Format: Red Green Blue - 8 bit - byte";
                case Texture::Format::RGB_8_UNSIGNED_INTEGER: return "Texture Format: Red Green Blue - 8 bit - unsigned byte";

                case Texture::Format::RGB_16: return "Texture Format: Red Green Blue - 16 but - unsigned byte";
                case Texture::Format::RGB_16_INTEGER: return "Texture Format: Red Green Blue - 16 bit - short";
                case Texture::Format::RGB_16_UNSIGNED_INTEGER: return "Texture Format: Red Green Blue - 16 bit - unsigned short";
                case Texture::Format::RGB_16_FLOAT: return "Texture Format: Red Green Blue - 16 bit - float";

                case Texture::Format::RGB_32_INTEGER: return "Texture Format: Red Green Blue - 32 bit - integer";
                case Texture::Format::RGB_32_UNSIGNED_INTEGER: return "Texture Format: Red Green Blue - 32 bit - unsigned integer";
                case Texture::Format::RGB_32_FLOAT: return "Texture Format: Red Green Blue - 32 bit - float";

                case Texture::Format::RGBA_8: return "Texture Format: Red Green Blue Alpha - 8 bit - unsigned byte";
                case Texture::Format::RGBA_8_INTEGER: return "Texture Format: Red Green Blue Alpha - 8 bit - byte";
                case Texture::Format::RGBA_8_UNSIGNED_INTEGER: return "Texture Format: Red Green Blue Alpha - 8 bit - unsigned byte";

                case Texture::Format::RGBA_16: return "Texture Format: Red Green Blue Alpha - 16 bit - unsigned byte";
                case Texture::Format::RGBA_16_INTEGER: return "Texture Format: Red Green Blue Alpha - 16 bit - short";
                case Texture::Format::RGBA_16_UNSIGNED_INTEGER: return "Texture Format: Red Green Blue Alpha - 16 bit - unsigned short";
                case Texture::Format::RGBA_16_FLOAT: return "Texture Format: Red Green Blue Alpha - 16 bit - float";

                case Texture::Format::RGBA_32_INTEGER: return "Texture Format: Red Green Blue Alpha - 32 bit - integer";
                case Texture::Format::RGBA_32_UNSIGNED_INTEGER: return "Texture Format: Red Green Blue Alpha - 32 bit - unsigned integer";
                case Texture::Format::RGBA_32_FLOAT: return "Texture Format: Red Green Blue Alpha - 32 bit - float";

                case Texture::Format::DEPTH_COMPONENT_16_INTEGER: return "Texture Format: Depth Component - 16 bit - integer";
                case Texture::Format::DEPTH_COMPONENT_24_INTEGER: return "Texture Format: Depth Component - 24 bit - integer";
                case Texture::Format::DEPTH_COMPONENT_32_INTEGER: return "Texture Format: Depth Component - 32 bit - integer";
                case Texture::Format::DEPTH_COMPONENT_24_UNSIGNED_INTEGER_STENCIL_8_UNSIGNED_INTEGER: return "Texture Format: Depth Component - 24 bit ( depth ) + 8 bit ( stencil )  - unsigned integer";
                case Texture::Format::DEPTH_COMPONENT_32_FLOAT: return "Texture Format: Depth Component - 32 bit - float";
                case Texture::Format::DEPTH_COMPONENT_32_FLOAT_STENCIL_8_FLOAT: return "Texture Format: Depth Component - 24 bit ( depth ) + 8 bit ( stencil ) - float";
            }

            R_ASSERT_X(false, "Unreachable code");
            return "";
        }
    }  // namespace conversions

    //-------------------------------------------------------------------------
    const std::vector<rex::Texture::Wrap>& Texture::default_texture_2D_wrapping()
    {
        return g_default_2D_texture_wrap;
    }
    //-------------------------------------------------------------------------
    const std::vector<rex::Texture::Wrap>& Texture::default_texture_3D_wrapping()
    {
        return g_default_3D_texture_wrap;
    }

    //-------------------------------------------------------------------------
    const std::vector<rex::Texture::Filter>& Texture::default_texture_filter()
    {
        return g_default_texture_filter;
    }

    //-------------------------------------------------------------------------
    const uint32 Texture::get_texture_format_byte_size(const Texture::Format& format)
    {
        switch (format)
        {
            case Texture::Format::UNKNOWN: return 0;

            case Texture::Format::R_8: return sizeof(uint8);
            case Texture::Format::R_8_INTEGER: return sizeof(int8);
            case Texture::Format::R_8_UNSIGNED_INTEGER: return sizeof(uint8);

            case Texture::Format::R_16: return sizeof(uint16);
            case Texture::Format::R_16_INTEGER: return sizeof(int16);
            case Texture::Format::R_16_UNSIGNED_INTEGER: return sizeof(uint16);
            case Texture::Format::R_16_FLOAT: return gsl::narrow<uint32>(sizeof(float) / 2.0f);

            case Texture::Format::R_32_INTEGER: return sizeof(int32);
            case Texture::Format::R_32_UNSIGNED_INTEGER: return sizeof(uint32);
            case Texture::Format::R_32_FLOAT: return sizeof(float);

            case Texture::Format::RG_8: return (sizeof(uint8)) * 2;
            case Texture::Format::RG_8_INTEGER: return (sizeof(int8)) * 2;
            case Texture::Format::RG_8_UNSIGNED_INTEGER: return (sizeof(uint8)) * 2;

            case Texture::Format::RG_16: return (sizeof(uint16)) * 2;
            case Texture::Format::RG_16_INTEGER: return (sizeof(int16)) * 2;
            case Texture::Format::RG_16_UNSIGNED_INTEGER: return (sizeof(uint16)) * 2;
            case Texture::Format::RG_16_FLOAT: return gsl::narrow<uint32>(sizeof(float) / 2.0f) * 2;

            case Texture::Format::RG_32_INTEGER: return (sizeof(int32)) * 2;
            case Texture::Format::RG_32_UNSIGNED_INTEGER: return (sizeof(uint32)) * 2;
            case Texture::Format::RG_32_FLOAT: return (sizeof(float)) * 2;

            case Texture::Format::RGB_8: return (sizeof(uint8)) * 3;
            case Texture::Format::RGB_8_INTEGER: return (sizeof(int8)) * 3;
            case Texture::Format::RGB_8_UNSIGNED_INTEGER: return (sizeof(uint8)) * 3;

            case Texture::Format::RGB_16: return (sizeof(uint16)) * 3;
            case Texture::Format::RGB_16_INTEGER: return (sizeof(int16)) * 3;
            case Texture::Format::RGB_16_UNSIGNED_INTEGER: return (sizeof(uint16)) * 3;
            case Texture::Format::RGB_16_FLOAT: return gsl::narrow<uint32>(sizeof(float) / 2.0f) * 3;

            case Texture::Format::RGB_32_INTEGER: return (sizeof(int32)) * 3;
            case Texture::Format::RGB_32_UNSIGNED_INTEGER: return (sizeof(uint32)) * 3;
            case Texture::Format::RGB_32_FLOAT: return (sizeof(float)) * 3;

            case Texture::Format::RGBA_8: return (sizeof(uint8)) * 4;
            case Texture::Format::RGBA_8_INTEGER: return (sizeof(int8)) * 4;
            case Texture::Format::RGBA_8_UNSIGNED_INTEGER: return (sizeof(uint8)) * 4;

            case Texture::Format::RGBA_16: return (sizeof(uint16)) * 4;
            case Texture::Format::RGBA_16_INTEGER: return (sizeof(int16)) * 4;
            case Texture::Format::RGBA_16_UNSIGNED_INTEGER: return (sizeof(uint16)) * 4;
            case Texture::Format::RGBA_16_FLOAT: return gsl::narrow<uint32>(sizeof(float) / 2.0f) * 4;

            case Texture::Format::RGBA_32_INTEGER: return (sizeof(int32)) * 4;
            case Texture::Format::RGBA_32_UNSIGNED_INTEGER: return (sizeof(uint32)) * 4;
            case Texture::Format::RGBA_32_FLOAT: return (sizeof(float)) * 4;

            case Texture::Format::DEPTH_COMPONENT_16_INTEGER: return sizeof(int16);
            case Texture::Format::DEPTH_COMPONENT_24_INTEGER: return sizeof(int16) + sizeof(int8);
            case Texture::Format::DEPTH_COMPONENT_32_INTEGER: return sizeof(int32);
            case Texture::Format::DEPTH_COMPONENT_24_UNSIGNED_INTEGER_STENCIL_8_UNSIGNED_INTEGER: return sizeof(uint32);
            case Texture::Format::DEPTH_COMPONENT_32_FLOAT: return sizeof(float);
            case Texture::Format::DEPTH_COMPONENT_32_FLOAT_STENCIL_8_FLOAT: return sizeof(float);
        }

        R_ASSERT_X(false, "Unreachable code");
        return 0;
    }

    //-------------------------------------------------------------------------
    uint32 Texture::generate_mip_count() const
    {
        return (uint32)std::floor(std::log2(std::min(get_width(), get_height()))) + 1;
    }
}  // namespace rex