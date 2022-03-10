#pragma once

#include "resources/texture.h"

namespace rex
{
    namespace opengl
    {
        uint32 to_opengl_texelformat(const Texel::Format &texelFormat);
        uint32 to_opengl_textureformat(const Texture::Format &textureFormat);
        uint32 to_opengl_pixeltype(const Pixel::Type &pixelType);
    }
}