#pragma once

#include "resources/shader_attribute_type.h"

namespace rex
{
    class ShaderAttribute
    {
    public:
        static ShaderAttribute EMPTY;

        ShaderAttribute();
        ShaderAttribute(const StringID& name, ShaderAttributeType type, uint32 size, uint32 offset);
        ~ShaderAttribute();

        const StringID& get_name() const;
        const ShaderAttributeType& get_type() const;

        uint32 get_size() const;
        uint32 get_offset() const;

        bool is_valid() const;

    private:
        StringID m_name;
        ShaderAttributeType m_type;
        uint32 m_size;
        uint32 m_offset;
    };

    using ShaderAttributes = std::vector<ShaderAttribute>;
}