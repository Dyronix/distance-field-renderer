#include "regina_pch.h"

#include "volume_library.h"
#include "volume_meta.h"
#include "volume.h"

#include "resources/resource_factory.h"
#include "resources/texture_3d_description.h"
#include "resources/texture_3d.h"

namespace regina
{
    namespace volume_library
    {
        //-------------------------------------------------------------------------
        Volume& get_empty_volume()
        {
            static Volume empty_volume;

            return empty_volume;
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Texture>& get_empty_volume_data()
        {
            static rex::ref_ptr<rex::Texture> empty_volume_data;

            return empty_volume_data;
        }

        //-------------------------------------------------------------------------
        rex::Texture3DDescription create_texture_description_3d_from(const Volume& volume)
        {
            rex::Texture3DDescription description;

            description.width = (uint32)volume.get_voxel_grid_size().x;
            description.height = (uint32)volume.get_voxel_grid_size().y;
            description.depth = (uint32)volume.get_voxel_grid_size().z;

            description.wraps =
            {
                { rex::Texture::Wrap::Coordinate::WRAP_R, rex::Texture::Wrap::Type::CLAMP_TO_EDGE },
                { rex::Texture::Wrap::Coordinate::WRAP_S, rex::Texture::Wrap::Type::CLAMP_TO_EDGE },
                { rex::Texture::Wrap::Coordinate::WRAP_T, rex::Texture::Wrap::Type::CLAMP_TO_EDGE },
            };

            description.filters =
            {
                { rex::Texture::Filter::Action::MINIFICATION, rex::Texture::Filter::Type::LINEAR },
                { rex::Texture::Filter::Action::MAGNIFICATION, rex::Texture::Filter::Type::LINEAR },
            };

            description.format = rex::Texture::Format::R_32_FLOAT;
            description.texel_format = rex::Texel::Format::R;
            description.usage = rex::Texture::Usage::UNSPECIFIED;

            rex::memory::Blob::copy(volume.get_volume_data(), description.data);

            return description;
        }

        std::unordered_map<rex::StringID, Volume> g_volumes;
        std::unordered_map<rex::StringID, rex::ref_ptr<rex::Texture>> g_textures;

        //-------------------------------------------------------------------------
        bool add(Volume&& volume)
        {
            auto it = g_volumes.find(volume.get_name());
            if (it != std::cend(g_volumes))
            {
                return false;
            }

            rex::StringID volume_name = volume.get_name();

            rex::Texture3DDescription texture_3d_description = create_texture_description_3d_from(volume);
            rex::ref_ptr<rex::Texture> texture_3d = rex::ResourceFactory::create_texture3d(std::move(texture_3d_description));

            g_volumes.insert(std::make_pair(volume_name, std::move(volume)));
            g_textures.insert(std::make_pair(volume_name, texture_3d));

            return true;
        }

        //-------------------------------------------------------------------------
        Volume& get_volume(const rex::StringID& volumeName)
        {
            auto it = g_volumes.find(volumeName);

            if(it != std::cend(g_volumes))
            {
                return it->second;
            }
                
            R_WARN("Volume with name: {0}, was not found", volumeName.to_string());

            return get_empty_volume();
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Texture>& get_volume_data(const rex::StringID& volumeName)
        {
            auto it = g_textures.find(volumeName);

            if (it != std::cend(g_textures))
            {
                return it->second;
            }

            R_WARN("Volume Data with name: {0}, was not found", volumeName.to_string());
                
            return get_empty_volume_data();
        }

        //-------------------------------------------------------------------------
        void clear()
        {
            g_volumes.clear();

            for(auto& pair : g_textures)
            {
                pair.second->release();
            }

            g_textures.clear();
        }
    }
}