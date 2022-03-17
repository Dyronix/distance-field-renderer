#include "regina_pch.h"

#include "texture_importer.h"

#include "resources/texture_2d_description.h"
#include "resources/resource_factory.h"

#include "file_import.h"
#include "file_memory.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace regina
{
    namespace texture_importer
    {
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Texture> import(const rex::StringID& name, const rex::StringID& path, const SRGB& srgb, const rex::Texture::Usage& usage)
        {
            rex::FileMemory loaded_data = rex::read_from_disk(path.to_string(), rex::ReadBinary::YES, rex::NullTerminate::NO);

            int32 width = 0;
            int32 height = 0;
            int32 channels = 0;

            int32 desired_channels = srgb ? STBI_rgb : STBI_rgb_alpha;

            //
            // tell stb_image.h to flip loaded texture's on the y-axis.
            //
            stbi_set_flip_vertically_on_load(true); 

            //
            // used to retrieve width, height and channels of the image.
            //
            auto* image_data = stbi_load_from_memory(loaded_data.get_data_as<stbi_uc>(), gsl::narrow<int32>(loaded_data.get_size()), &width, &height, &channels, desired_channels);
            if (!image_data)
            {
                R_ERROR("STBI: {0}", stbi_failure_reason());

                return nullptr;
            }

            rex::Texture2DDescription desc;

            desc.name = name;
            desc.width = width;
            desc.height = height;
            desc.texel_format = channels == STBI_rgb ? rex::Texel::Format::RGB : rex::Texel::Format::RGBA;
            desc.format = channels == STBI_rgb ? rex::Texture::Format::RGB_8 : rex::Texture::Format::RGBA_8;
            desc.usage = usage;
            desc.data = rex::memory::make_blob((rex::memory::byte*)image_data, width * height * rex::Texel::get_channel_count(desc.texel_format));

            return rex::ResourceFactory::create_texture2d(std::move(desc));
        }
    }
}