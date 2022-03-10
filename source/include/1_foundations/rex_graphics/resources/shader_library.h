#pragma once

#include "string/stringid.h"

#include <vector>
#include <unordered_map>

namespace sbt
{
    class ShaderProgram;

    namespace shader_library
    {
        bool                                    add(const ref_ptr<sbt::ShaderProgram>& program);
        bool                                    add(const std::vector<ref_ptr<sbt::ShaderProgram>>& programs);
         
        ref_ptr<sbt::ShaderProgram>             get(const StringID& programName);
        std::vector<ref_ptr<ShaderProgram>>     getAll();

        void                                    clear();
    };
}