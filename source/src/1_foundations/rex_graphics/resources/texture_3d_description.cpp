#include "rex_graphics_pch.h"

#include "resources/texture_3d_description.h"

namespace rex
{
        //-------------------------------------------------------------------------
        Texture3DDescription::Texture3DDescription()
            : name(ESID::SID_None)
            , width(0)
            , height(0)
            , depth(0)
            , texel_format(Texel::Format::UNKNOWN)
            , usage(Texture::Usage::UNSPECIFIED)
            , format(Texture::Format::UNKNOWN)
            , wraps()
            , filters()
            , data()
        {
        }
        //-------------------------------------------------------------------------
        Texture3DDescription::Texture3DDescription(Texture3DDescription&& other) noexcept 
            : name(std::move(other.name))
            , width(std::move(other.width))
            , height(std::move(other.height))
            , depth(std::move(other.depth))
            , texel_format(std::move(other.texel_format))
            , usage(std::move(other.usage))
            , format(std::move(other.format))
            , wraps(std::move(other.wraps))
            , filters(std::move(other.filters))
            , data(std::move(other.data))
        {
        }

        //-------------------------------------------------------------------------
        Texture3DDescription& Texture3DDescription::operator=(Texture3DDescription&& other) noexcept
        {
            // Guard self assignment
            if (this == &other)
            {
                return *this;
            }

            this->name = std::move(other.name);

            this->width = std::move(other.width);
            this->height = std::move(other.height);
            this->depth = std::move(other.depth);

            this->texel_format = std::move(other.texel_format);

            this->usage = std::move(other.usage);
            this->format = std::move(other.format);

            this->wraps = std::move(other.wraps);
            this->filters = std::move(other.filters);

            this->data = std::move(other.data);

            return *this;
        }
}