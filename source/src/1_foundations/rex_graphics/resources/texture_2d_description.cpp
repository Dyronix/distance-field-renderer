#include "rex_graphics_pch.h"

#include "resources/texture_2d_description.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Texel::Format get_texel_format(Texture::Format textureFormat)
    {
        switch (textureFormat)
        {
            case Texture::Format::R_8:
            case Texture::Format::R_8_INTEGER:
            case Texture::Format::R_8_UNSIGNED_INTEGER:
            case Texture::Format::R_16:
            case Texture::Format::R_16_INTEGER:
            case Texture::Format::R_16_UNSIGNED_INTEGER:
            case Texture::Format::R_16_FLOAT:
            case Texture::Format::R_32_INTEGER:
            case Texture::Format::R_32_UNSIGNED_INTEGER:
            case Texture::Format::R_32_FLOAT: return Texel::Format::R;

            case Texture::Format::RG_8:
            case Texture::Format::RG_8_INTEGER:
            case Texture::Format::RG_8_UNSIGNED_INTEGER:
            case Texture::Format::RG_16:
            case Texture::Format::RG_16_INTEGER:
            case Texture::Format::RG_16_UNSIGNED_INTEGER:
            case Texture::Format::RG_16_FLOAT:
            case Texture::Format::RG_32_INTEGER:
            case Texture::Format::RG_32_UNSIGNED_INTEGER:
            case Texture::Format::RG_32_FLOAT: return Texel::Format::RG;

            case Texture::Format::RGB_8:
            case Texture::Format::RGB_8_INTEGER:
            case Texture::Format::RGB_8_UNSIGNED_INTEGER:
            case Texture::Format::RGB_16:
            case Texture::Format::RGB_16_INTEGER:
            case Texture::Format::RGB_16_UNSIGNED_INTEGER:
            case Texture::Format::RGB_16_FLOAT:
            case Texture::Format::RGB_32_INTEGER:
            case Texture::Format::RGB_32_UNSIGNED_INTEGER:
            case Texture::Format::RGB_32_FLOAT: return Texel::Format::RGB;

            case Texture::Format::RGBA_8:
            case Texture::Format::RGBA_8_INTEGER:
            case Texture::Format::RGBA_8_UNSIGNED_INTEGER:
            case Texture::Format::RGBA_16:
            case Texture::Format::RGBA_16_INTEGER:
            case Texture::Format::RGBA_16_UNSIGNED_INTEGER:
            case Texture::Format::RGBA_16_FLOAT:
            case Texture::Format::RGBA_32_INTEGER:
            case Texture::Format::RGBA_32_UNSIGNED_INTEGER:
            case Texture::Format::RGBA_32_FLOAT: return Texel::Format::RGBA;

            case Texture::Format::DEPTH_COMPONENT_16_INTEGER:
            case Texture::Format::DEPTH_COMPONENT_24_INTEGER:
            case Texture::Format::DEPTH_COMPONENT_32_INTEGER:
            case Texture::Format::DEPTH_COMPONENT_32_FLOAT: return Texel::Format::DEPTH_COMPONENT;

            case Texture::Format::DEPTH_COMPONENT_24_UNSIGNED_INTEGER_STENCIL_8_UNSIGNED_INTEGER:
            case Texture::Format::DEPTH_COMPONENT_32_FLOAT_STENCIL_8_FLOAT: return Texel::Format::DEPTH_STENCIL_COMPONENT;

            default: R_ASSERT_X(false, "Unsupported Texture Format"); return Texel::Format::UNKNOWN;
        }
    }

    //-------------------------------------------------------------------------
    Texture2DDescription create_depth_attachment_description(uint32 width, uint32 height, Texture::Format textureFormat)
    {
        Texture2DDescription desc;

        desc.name = "DepthAttachment"_sid;
        desc.width = width;
        desc.height = height;

        desc.texel_format = get_texel_format(textureFormat);
        desc.usage = Texture::Usage::DEPTH_ATTACHMENT;
        desc.format = textureFormat;
        desc.wraps = Texture::default_texture_2D_wrapping();
        desc.filters = Texture::default_texture_filter();

        return desc;
    }
    //-------------------------------------------------------------------------
    Texture2DDescription create_color_attachment_description(uint32 width, uint32 height, Texture::Format textureFormat)
    {
        Texture2DDescription desc;

        desc.name = "ColorAttachment"_sid;
        desc.width = width;
        desc.height = height;

        desc.texel_format = get_texel_format(textureFormat);
        desc.usage = Texture::Usage::COLOR_ATTACHMENT;
        desc.format = textureFormat;
        desc.wraps = Texture::default_texture_2D_wrapping();
        desc.filters = Texture::default_texture_filter();

        return desc;
    }
}