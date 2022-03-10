#pragma once

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/shaderuniformtype.h"

namespace sbt
{
    class ShaderUniform
    {
    public:
        ShaderUniform();
        ShaderUniform(const StringID& name, const ShaderUniformType& type, uint32 size, uint32 offset);

        const StringID& getName() const;
        const ShaderUniformType& getType() const;

        uint32 getSize() const;
        uint32 getOffset() const;

        bool isValid() const;

        static ShaderUniform EMPTY;

    private:
        StringID m_name;
        ShaderUniformType m_type;
        uint32 m_size;
        uint32 m_offset;
    };

    using ShaderUniforms = std::vector<ShaderUniform>;
}