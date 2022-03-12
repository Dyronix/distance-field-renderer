#pragma once

#include "resources/shader_uniform.h"

namespace rex
{
    class ShaderUniformBlock
    {
    public:
        static ShaderUniformBlock EMPTY;

        ShaderUniformBlock();
        ShaderUniformBlock(const StringID& name, uint32 size, uint32 binding, const ShaderUniforms& uniforms);
        ~ShaderUniformBlock();

        const StringID& get_name() const;
        const ShaderUniforms& get_uniforms() const;

        uint32 get_size() const;
        uint32 get_binding() const;
        uint32 get_uniform_count() const;

        bool is_valid() const;

    private:
        StringID m_name;
        uint32 m_size;
        uint32 m_binding;
        ShaderUniforms m_uniforms;
    };

    using ShaderUniformBlocks = std::vector<ShaderUniformBlock>;
}