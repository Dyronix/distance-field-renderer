#pragma once

#include "resources/texture.h"

namespace rex
{
    struct TextureCubeDescription
    {
        TextureCubeDescription();
        TextureCubeDescription(TextureCubeDescription&& other) noexcept;
        TextureCubeDescription& operator=(TextureCubeDescription&& other) noexcept;

        StringID name;

        uint32 width;
        uint32 height;

        Texel texel_format;

        Texture::Usage usage;
        Texture::Format format;

        std::vector<Texture::Wrap> wraps;
        std::vector<Texture::Filter> filters;

        Texture::Data data;
    };
}