#include "rex_graphics_pch.h"

#include "resources/texture_library.h"
#include "resources/texture.h"

namespace rex
{
    namespace texture_library
    {
        std::unordered_map<StringID, ref_ptr<Texture>> g_textures;

        //-------------------------------------------------------------------------
        bool add(const ref_ptr<rex::Texture>& program)
        {
            auto it = g_textures.find(program->get_name());
            if (it != std::cend(g_textures))
            {
                return false;
            }

            g_textures.insert(std::make_pair(program->get_name(), program));
            return true;
        }
        //-------------------------------------------------------------------------
        bool add(const std::vector< ref_ptr<rex::Texture>>& programs)
        {
            bool result = true;
            for (auto& p : programs)
            {
                result &= add(p);
            }

            return result;
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> get(const StringID& programName)
        {
            auto it = g_textures.find(programName);

            return it != std::cend(g_textures)
                ? it->second
                : nullptr;
        }

        //-------------------------------------------------------------------------
        void clear()
        {
            for (auto& pair : g_textures)
            {
                pair.second->release();
            }

            g_textures.clear();
        }
    }
}