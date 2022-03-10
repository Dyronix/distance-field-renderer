#pragma once

#define UNSUPPORTED 0

#include "resources/texture.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        uint32 to_opengl_texelformat(const Texel::Format &texelFormat)
        {
            switch (texelFormat)
            {
            case Texel::Format::R:
                return GL_RED;
            case Texel::Format::RG:
                return GL_RG;
            case Texel::Format::RGB:
                return GL_RGB;
            case Texel::Format::RGBA:
                return GL_RGBA;

            case Texel::Format::DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT;
            case Texel::Format::DEPTH_STENCIL_COMPONENT:
                return GL_DEPTH_STENCIL;

            default:
                S_ASSERT_X(false, "Unsupported texel format");
                return 0;
            }

            S_ASSERT_X(false, "Invalid texel format");
            return 0;
        }
        //-------------------------------------------------------------------------
        uint32 to_opengl_textureformat(const Texture::Format &textureFormat)
        {
            switch (textureFormat)
            {
            case Texture::Format::R_8:
                return GL_R8;
            case Texture::Format::R_8_INTEGER:
                return GL_R8I;
            case Texture::Format::R_8_UNSIGNED_INTEGER:
                return GL_R8UI;
            case Texture::Format::R_16:
                return GL_R16;

            case Texture::Format::R_16_INTEGER:
                return GL_R16I;
            case Texture::Format::R_16_UNSIGNED_INTEGER:
                return GL_R16UI;
            case Texture::Format::R_16_FLOAT:
                return GL_R16F;
            case Texture::Format::R_32_INTEGER:
                return GL_R32I;

            case Texture::Format::R_32_UNSIGNED_INTEGER:
                return GL_R32UI;
            case Texture::Format::R_32_FLOAT:
                return GL_R32F;
            case Texture::Format::RG_8:
                return GL_RG8;

            case Texture::Format::RG_8_INTEGER:
                return GL_RG8I;
            case Texture::Format::RG_8_UNSIGNED_INTEGER:
                return GL_RG8UI;

            case Texture::Format::RG_16:
                return GL_RG16;
            case Texture::Format::RG_16_INTEGER:
                return GL_RG16I;
            case Texture::Format::RG_16_UNSIGNED_INTEGER:
                return GL_RG16UI;
            case Texture::Format::RG_16_FLOAT:
                return GL_RG16F;
            case Texture::Format::RG_32_INTEGER:
                return GL_RG32I;
            case Texture::Format::RG_32_UNSIGNED_INTEGER:
                return GL_RG32UI;
            case Texture::Format::RG_32_FLOAT:
                return GL_RG32F;
            case Texture::Format::RGB_8:
                return GL_RGB8;

            case Texture::Format::RGB_8_INTEGER:
                return GL_RGB8I;
            case Texture::Format::RGB_8_UNSIGNED_INTEGER:
                return GL_RGB8UI;

            case Texture::Format::RGB_16:
                return GL_RGB16;
            case Texture::Format::RGB_16_INTEGER:
                return GL_RGB16I;
            case Texture::Format::RGB_16_UNSIGNED_INTEGER:
                return GL_RGB16UI;
            case Texture::Format::RGB_16_FLOAT:
                return GL_RGB16F;

            case Texture::Format::RGB_32_INTEGER:
                return GL_RGB32I;
            case Texture::Format::RGB_32_UNSIGNED_INTEGER:
                return GL_RGB32UI;
            case Texture::Format::RGB_32_FLOAT:
                return GL_RGB32F;

            case Texture::Format::RGBA_8:
                return GL_RGBA8;
            case Texture::Format::RGBA_8_INTEGER:
                return GL_RGBA8I;
            case Texture::Format::RGBA_8_UNSIGNED_INTEGER:
                return GL_RGBA8UI;

            case Texture::Format::RGBA_16:
                return GL_RGBA16;
            case Texture::Format::RGBA_16_INTEGER:
                return GL_RGBA16I;
            case Texture::Format::RGBA_16_UNSIGNED_INTEGER:
                return GL_RGBA16UI;
            case Texture::Format::RGBA_16_FLOAT:
                return GL_RGBA16F;

            case Texture::Format::RGBA_32_INTEGER:
                return GL_RGBA32I;
            case Texture::Format::RGBA_32_UNSIGNED_INTEGER:
                return GL_RGBA32UI;
            case Texture::Format::RGBA_32_FLOAT:
                return GL_RGBA32F;
            case Texture::Format::DEPTH_COMPONENT_16_INTEGER:
                return GL_DEPTH_COMPONENT16;
            case Texture::Format::DEPTH_COMPONENT_24_INTEGER:
                return GL_DEPTH_COMPONENT24;

            case Texture::Format::DEPTH_COMPONENT_32_INTEGER:
                return GL_DEPTH_COMPONENT32;
            case Texture::Format::DEPTH_COMPONENT_32_FLOAT:
                return GL_DEPTH_COMPONENT32F;
            case Texture::Format::DEPTH_COMPONENT_24_UNSIGNED_INTEGER_STENCIL_8_UNSIGNED_INTEGER:
                return GL_DEPTH24_STENCIL8;
            case Texture::Format::DEPTH_COMPONENT_32_FLOAT_STENCIL_8_FLOAT:
                return GL_DEPTH32F_STENCIL8;

            default:
                S_ASSERT_X(false, "Unsupported texture format");
                return 0;
            }

            S_ASSERT_X(false, "Invalid texture format");
            return 0;
        }
        //-------------------------------------------------------------------------
        uint32 to_opengl_pixeltype(const Pixel::Type &pixelType)
        {
            switch (pixelType)
            {
            case Pixel::Type::UNSIGNED_BYTE:
                return GL_UNSIGNED_BYTE;
            case Pixel::Type::BYTE:
                return GL_BYTE;

            case Pixel::Type::UNSIGNED_SHORT:
                return GL_UNSIGNED_SHORT;
            case Pixel::Type::SHORT:
                return GL_SHORT;

            case Pixel::Type::INT:
                return GL_INT;
            case Pixel::Type::UNSIGNED_INT:
                return GL_UNSIGNED_INT;

            case Pixel::Type::HALF_FLOAT:
                return GL_HALF_FLOAT;
            case Pixel::Type::FLOAT:
                return GL_FLOAT;

            case Pixel::Type::UNSIGNED_INT_24_8:
                return GL_UNSIGNED_INT_24_8;
            case Pixel::Type::FLOAT_32_UNSIGNED_INT_24_8_REV:
                return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

            default:
                S_ASSERT_X(false, "Unsupported pixel type");
                return 0;
            }

            S_ASSERT_X(false, "Invalid pixel type");
            return 0;
        }
    }
}