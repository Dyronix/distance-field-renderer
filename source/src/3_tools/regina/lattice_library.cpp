#include "regina_pch.h"

#include "lattice_library.h"
#include "lattice_meta.h"
#include "lattice.h"

#include "resources/resource_factory.h"
#include "resources/texture_3d.h"
#include "resources/texture_3d_description.h"

namespace regina
{
    namespace lattice_library
    {
        //-------------------------------------------------------------------------
        Lattice& get_empty_lattice()
        {
            static Lattice empty_lattice;

            return empty_lattice;
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Texture>& get_empty_lattice_data()
        {
            static rex::ref_ptr<rex::Texture> empty_lattice_data;

            return empty_lattice_data;
        }

        //-------------------------------------------------------------------------
        rex::Texture3DDescription create_texture_description_3d_from(const Lattice& lattice)
        {
            rex::Texture3DDescription description;

            description.width = (uint32)lattice.get_lattice_grid_size().x;
            description.height = (uint32)lattice.get_lattice_grid_size().y;
            description.depth = (uint32)lattice.get_lattice_grid_size().z;

            description.wraps = {
                {rex::Texture::Wrap::Coordinate::WRAP_R, rex::Texture::Wrap::Type::CLAMP_TO_EDGE},
                {rex::Texture::Wrap::Coordinate::WRAP_S, rex::Texture::Wrap::Type::CLAMP_TO_EDGE},
                {rex::Texture::Wrap::Coordinate::WRAP_T, rex::Texture::Wrap::Type::CLAMP_TO_EDGE},
            };

            description.filters = {
                {rex::Texture::Filter::Action::MINIFICATION, rex::Texture::Filter::Type::LINEAR},
                {rex::Texture::Filter::Action::MAGNIFICATION, rex::Texture::Filter::Type::LINEAR},
            };

            description.format = rex::Texture::Format::R_32_FLOAT;
            description.texel_format = rex::Texel::Format::R;
            description.usage = rex::Texture::Usage::UNSPECIFIED;

            rex::memory::Blob::copy(lattice.get_lattice_data(), description.data);

            return description;
        }

        std::unordered_map<rex::StringID, Lattice> g_lattices;
        std::unordered_map<rex::StringID, rex::ref_ptr<rex::Texture>> g_textures;

        //-------------------------------------------------------------------------
        bool add(Lattice&& lattice)
        {
            auto it = g_lattices.find(lattice.get_name());
            if (it != std::cend(g_lattices))
            {
                return false;
            }

            rex::StringID lattice_name = lattice.get_name();

            rex::Texture3DDescription texture_3d_description = create_texture_description_3d_from(lattice);
            rex::ref_ptr<rex::Texture> texture_3d = rex::ResourceFactory::create_texture3d(std::move(texture_3d_description));

            g_lattices.insert(std::make_pair(lattice_name, std::move(lattice)));
            g_textures.insert(std::make_pair(lattice_name, texture_3d));

            return true;
        }

        //-------------------------------------------------------------------------
        regina::Lattice& get_lattice(const rex::StringID& latticeName)
        {
            auto it = g_lattices.find(latticeName);

            return it != std::cend(g_lattices) 
                ? it->second 
                : get_empty_lattice();
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Texture>& get_lattice_data(const rex::StringID& latticeName)
        {
            auto it = g_textures.find(latticeName);

            return it != std::cend(g_textures) 
                ? it->second 
                : get_empty_lattice_data();
        }

        //-------------------------------------------------------------------------
        void clear()
        {
            g_lattices.clear();

            for (auto& pair : g_textures)
            {
                pair.second->release();
            }

            g_textures.clear();
        }

    } // namespace lattice_library
}