#include "rex_graphics_pch.h"

#include "resources/shader_library.h"
#include "resources/shader_program.h"

namespace rex
{
    namespace shader_library
    {
        std::unordered_map<StringID, ref_ptr<ShaderProgram>> g_programs;

        //-------------------------------------------------------------------------
        bool add(const ref_ptr<rex::ShaderProgram>& program)
        {
            auto it = g_programs.find(program->get_name());
            if (it != std::cend(g_programs))
            {
                return false;
            }

            g_programs.insert(std::make_pair(program->get_name(), program));
            return true;
        }
        //-------------------------------------------------------------------------
        bool add(const std::vector< ref_ptr<rex::ShaderProgram>>& programs)
        {
            bool result = true;
            for (auto& p : programs)
            {
                result &= add(p);
            }

            return result;
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::ShaderProgram> get(const StringID& programName)
        {
            auto it = g_programs.find(programName);

            return it != std::cend(g_programs)
                ? it->second
                : nullptr;
        }

        //-------------------------------------------------------------------------
        std::vector<rex::ref_ptr<rex::ShaderProgram>> getAll()
        {
            std::vector<ref_ptr<ShaderProgram>> programs;
            std::transform(std::cbegin(g_programs), std::cend(g_programs), std::back_inserter(programs),
                [](const auto& pair)
                {
                    return pair.second;
                });

            return programs;
        }

        //-------------------------------------------------------------------------
        void clear()
        {
            for (auto& pair : g_programs)
            {
                pair.second->release();
            }

            g_programs.clear();
        }
    }
}