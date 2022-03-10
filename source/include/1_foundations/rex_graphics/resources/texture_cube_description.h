#pragma once

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/texture.h"

#include <memory>

namespace sbt
{
    struct TextureCubeDescription
    {
        //-------------------------------------------------------------------------
        TextureCubeDescription()
            : name(EName::SID_None)
            , width(0)
            , height(0)
            , texel_format(Texel::Format::UNKNOWN)
            , usage(Texture::Usage::UNSPECIFIED)
            , format(Texture::Format::UNKNOWN)
            , wraps()
            , filters()
            , data()
        {
        }
        //-------------------------------------------------------------------------
        TextureCubeDescription(TextureCubeDescription&& other) noexcept : name(std::move(other.name)),
                                                                          width(std::move(other.width)),
                                                                          height(std::move(other.height)),
                                                                          texel_format(std::move(other.texel_format)),
                                                                          usage(std::move(other.usage)),
                                                                          format(std::move(other.format)),
                                                                          wraps(std::move(other.wraps)),
                                                                          filters(std::move(other.filters)),
                                                                          data(std::move(other.data))
        {
        }

        //-------------------------------------------------------------------------
        TextureCubeDescription& operator=(TextureCubeDescription&& other) noexcept
        {
            // Guard self assignment
            if (this == &other)
            {
                return *this;
            }

            this->name = std::move(other.name);
            this->width = std::move(other.width);
            this->height = std::move(other.height);

            this->texel_format = std::move(other.texel_format);

            this->usage = std::move(other.usage);
            this->format = std::move(other.format);

            this->wraps = std::move(other.wraps);
            this->filters = std::move(other.filters);

            this->data = std::move(other.data);

            return *this;
        }

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