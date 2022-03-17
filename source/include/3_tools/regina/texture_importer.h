#pragma once

#include "yes_no_enum.h"

#include "resources/texture.h"

namespace regina
{
    using SRGB = rex::YesNoEnum;

    namespace texture_importer
    {
        rex::ref_ptr<rex::Texture> import(const rex::StringID& name, const rex::StringID& path, const SRGB& srgb = SRGB::NO, const rex::Texture::Usage& usage = rex::Texture::Usage::UNSPECIFIED);
    }
}