#pragma once

#include "resources/shader_uniform_type.h"

namespace rex
{
    class ShaderUniform
    {
    public:
        static ShaderUniform EMPTY;

        ShaderUniform();
        ShaderUniform(const StringID& name, const ShaderUniformType& type, uint32 size, uint32 offset);

        const StringID& get_name() const;
        const ShaderUniformType& get_type() const;

        uint32 get_size() const;
        uint32 get_offset() const;

        bool is_valid() const;

    private:
        StringID m_name;
        ShaderUniformType m_type;
        uint32 m_size;
        uint32 m_offset;
    };

    using ShaderUniforms = std::vector<ShaderUniform>;
}