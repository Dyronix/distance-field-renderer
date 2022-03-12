#pragma once

namespace rex
{
    class ShaderProgram;

    namespace shader_library
    {
        bool add(const ref_ptr<rex::ShaderProgram>& program);
        bool add(const std::vector<ref_ptr<rex::ShaderProgram>>& programs);

        ref_ptr<rex::ShaderProgram> get(const StringID& programName);
        std::vector<ref_ptr<ShaderProgram>> get_all();

        void clear();
    };
}