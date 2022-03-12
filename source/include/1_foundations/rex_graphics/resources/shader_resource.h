#pragma once

#include "resources/shader_uniform_type.h"

namespace rex
{
    class ShaderResource
    {
    public:
        static ShaderResource EMPTY;

        enum class Domain
        {
            NONE,
            VERTEX,
            PIXEL
        };

        ShaderResource();
        ShaderResource(const StringID& name, ShaderUniformType type, uint32 count, uint32 offset, uint32 reg);

        const StringID& get_name() const;

        const ShaderUniformType& get_type() const;
        const uint32 get_count() const;
        const uint32 get_offset() const;
        const uint32 get_register() const;

        bool is_valid() const;

    private:
        StringID m_name;
        ShaderUniformType m_type;
        uint32 m_count;
        uint32 m_offset;
        uint32 m_register;
    };

    using ShaderResources = std::vector<ShaderResource>;
}