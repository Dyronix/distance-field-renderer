#pragma once

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/shaderattributetype.h"

namespace sbt
{
    class ShaderAttribute
    {
    public:
        ShaderAttribute();
        ShaderAttribute(const StringID& name, ShaderAttributeType type, uint32 size, uint32 offset);
        ~ShaderAttribute();

        const StringID& getName() const;
        const ShaderAttributeType& getType() const;

        uint32 getSize() const;
        uint32 getOffset() const;

        bool isValid() const;

        static ShaderAttribute EMPTY;

    private:
        StringID m_name;
        ShaderAttributeType m_type;
        uint32 m_size;
        uint32 m_offset;
    };

    using ShaderAttributes = std::vector<ShaderAttribute>;
}