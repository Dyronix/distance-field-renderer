#pragma once

#include "color.h"
#include "blob.h"

#include "resources/api_resource.h"

namespace rex
{
    using rgba = uint32_t;

    class Texel
    {
    public:
        enum class Format
        {
            UNKNOWN,

            R,
            RG,
            RGB,
            RGBA,

            DEPTH_COMPONENT,
            DEPTH_STENCIL_COMPONENT,
        };

        static uint32 get_channel_count(const Texel::Format& format);

        Texel();
        Texel(const Texel::Format& format);
        ~Texel();

        uint32 get_count() const;

        operator uint32() const;
        operator Texel::Format() const;

        std::string to_string() const;

    private:
        Format m_format;
    };

    class Texture : public Resource
    {
    public:
        using Data = memory::Blob;

        struct Wrap
        {
            enum class Coordinate
            {
                WRAP_R,
                WRAP_S,
                WRAP_T
            };

            enum class Type
            {
                CLAMP_TO_BORDER,
                CLAMP_TO_EDGE,
                REPEAT
            };

            Coordinate coordinate;
            Type type;
        };

        struct Filter
        {
            enum class Action
            {
                MINIFICATION,
                MAGNIFICATION
            };

            enum class Type
            {
                NEAREST,
                LINEAR
            };

            Action action;
            Type type;
        };

        enum class Type
        {
            TEXTURE_2D,
            TEXTURE_3D,
            TEXTURE_CUBE,
            RENDER_TARGET
        };

        enum class Usage
        {
            UNSPECIFIED,

            DIFFUSE,
            SPECUALR,
            NORMAL,
            AMBIENT_OCCLUSION,
            ROUGHNESS,

            COLOR_ATTACHMENT,
            DEPTH_ATTACHMENT,

            CUBEMAP_ATTACHMENT
        };

        enum class Format
        {
            UNKNOWN,

            R_8,
            R_8_INTEGER,
            R_8_UNSIGNED_INTEGER,

            R_16,
            R_16_INTEGER,
            R_16_UNSIGNED_INTEGER,
            R_16_FLOAT,

            R_32_INTEGER,
            R_32_UNSIGNED_INTEGER,
            R_32_FLOAT,

            RG_8,
            RG_8_INTEGER,
            RG_8_UNSIGNED_INTEGER,

            RG_16,
            RG_16_INTEGER,
            RG_16_UNSIGNED_INTEGER,
            RG_16_FLOAT,

            RG_32_INTEGER,
            RG_32_UNSIGNED_INTEGER,
            RG_32_FLOAT,

            RGB_8,
            RGB_8_INTEGER,
            RGB_8_UNSIGNED_INTEGER,

            RGB_16,
            RGB_16_INTEGER,
            RGB_16_UNSIGNED_INTEGER,
            RGB_16_FLOAT,

            RGB_32_INTEGER,
            RGB_32_UNSIGNED_INTEGER,
            RGB_32_FLOAT,

            RGBA_8,
            RGBA_8_INTEGER,
            RGBA_8_UNSIGNED_INTEGER,

            RGBA_16,
            RGBA_16_INTEGER,
            RGBA_16_UNSIGNED_INTEGER,
            RGBA_16_FLOAT,

            RGBA_32_INTEGER,
            RGBA_32_UNSIGNED_INTEGER,
            RGBA_32_FLOAT,

            DEPTH_COMPONENT_16_INTEGER,
            DEPTH_COMPONENT_24_INTEGER,
            DEPTH_COMPONENT_32_INTEGER,
            DEPTH_COMPONENT_32_FLOAT,

            DEPTH_COMPONENT_24_UNSIGNED_INTEGER_STENCIL_8_UNSIGNED_INTEGER,
            DEPTH_COMPONENT_32_FLOAT_STENCIL_8_FLOAT,
        };

        static const std::vector<Texture::Wrap>& default_texture_2D_wrapping();
        static const std::vector<Texture::Wrap>& default_texture_3D_wrapping();

        static const std::vector<Texture::Filter>& default_texture_filter();

        static const uint32 get_texture_format_byte_size(const Texture::Format& format);

        virtual ~Texture() = default;

        virtual void invalidate() = 0;
        virtual void release() = 0;

        virtual const StringID& get_name() const = 0;
        virtual const Texture::Data& get_data() const = 0;

        virtual uint32 get_width() const = 0;
        virtual uint32 get_height() const = 0;

        virtual uint32 get_id() const = 0;

        virtual Texture::Type get_type() const = 0;
        virtual Texture::Format get_format() const = 0;
        virtual Texture::Usage get_usage() const = 0;
        virtual Texel::Format get_channels() const = 0;

        virtual void set_data(Texture::Data&& textureData) = 0;
        virtual void set_wrap(const Wrap& textureWrap) = 0;
        virtual void set_wrap_border_color(const ColorRGBA& color) = 0;
        virtual void set_filter(const Filter& textureFilter) = 0;
        virtual void set_format(const Texel& texelFormat, const Format& textureFormat) = 0;

        virtual void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;
        virtual void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;

    protected:
        Texture() = default;

        uint32 generate_mip_count() const;
    };

    class SolidColorTexture
    {
    public:
        SolidColorTexture(rgba textureData);

        void release();

        ref_ptr<Texture>& get_texture();
        const ref_ptr<Texture>& get_texture() const;

    private:
        ref_ptr<Texture> texture;
    };

    class Pixel
    {
    public:
        enum class Format
        {
            UNKNOWN,

            R,
            RG,
            RGB,
            RGBA,

            R_INTEGER,
            RG_INTEGER,
            RGB_INTEGER,
            RGBA_INTEGER,

            DEPTH_COMPONENT,
            DEPTH_STENCIL
        };

        enum class Type
        {
            UNKNOWN,

            UNSIGNED_BYTE,
            BYTE,

            UNSIGNED_SHORT,
            SHORT,

            INT,
            UNSIGNED_INT,

            HALF_FLOAT,
            FLOAT,

            UNSIGNED_INT_24_8,
            FLOAT_32_UNSIGNED_INT_24_8_REV
        };

        Pixel();
        Pixel(const Pixel& other);
        Pixel(const Texel::Format& texelFormat, const Texture::Format& textureFormat);
        Pixel(const Format& pixelFormat, const Type& pixelType);
        ~Pixel();

        const Format get_format() const;
        const Type get_type() const;

    private:
        Format m_format;
        Type m_type;
    };

    namespace conversions
    {
        std::string to_display_string(const Texture::Type& type);
        std::string to_display_string(const Texture::Usage& usage);
        std::string to_display_string(const Texture::Format& format);
    }
}