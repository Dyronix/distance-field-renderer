#pragma once

#include "resources/texture.h"

namespace rex
{
    struct Texture2DDescription
    {
        Texture2DDescription();
        Texture2DDescription(const Texture2DDescription& other) = delete;
        Texture2DDescription(Texture2DDescription&& other) noexcept;

        Texture2DDescription& operator=(Texture2DDescription& other) = delete;
        Texture2DDescription& operator=(Texture2DDescription&& other) noexcept;

        StringID name;

        int32 width;
        int32 height;

        Texel texel_format;

        Texture::Usage usage;
        Texture::Format format;

        std::vector<Texture::Wrap> wraps;
        std::vector<Texture::Filter> filters;

        Texture::Data data;
    };

    Texture2DDescription create_depth_attachment_description(uint32 width, uint32 height, Texture::Format textureFormat);
    Texture2DDescription create_color_attachment_Description(uint32 width, uint32 height, Texture::Format textureFormat);
}