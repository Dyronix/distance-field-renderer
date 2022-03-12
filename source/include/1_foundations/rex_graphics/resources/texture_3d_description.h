#pragma once

#include "resources/texture.h"

namespace rex
{
    struct Texture3DDescription
    {
        Texture3DDescription();
        Texture3DDescription(const Texture3DDescription& other) = delete;
        Texture3DDescription(Texture3DDescription&& other) noexcept;

        Texture3DDescription& operator=(Texture3DDescription& other) = delete;
        Texture3DDescription& operator=(Texture3DDescription&& other) noexcept;

        StringID name;

        uint32 width;
        uint32 height;
        uint32 depth;

        Texel texel_format;

        Texture::Usage usage;
        Texture::Format format;

        std::vector<Texture::Wrap> wraps;
        std::vector<Texture::Filter> filters;

        Texture::Data data;
    };
}