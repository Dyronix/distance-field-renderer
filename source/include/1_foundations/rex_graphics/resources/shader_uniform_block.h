#pragma once

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/shaderuniform.h"

namespace sbt
{
    class ShaderUniformBlock
    {
    public:
        ShaderUniformBlock();
        ShaderUniformBlock(const StringID& name, uint32 size, uint32 binding, const ShaderUniforms& uniforms);
        ~ShaderUniformBlock();

        const StringID& getName() const;
        const ShaderUniforms& getUniforms() const;

        uint32 getSize() const;
        uint32 getBinding() const;
        uint32 getUniformCount() const;

        bool isValid() const;

        static ShaderUniformBlock EMPTY;

    private:
        StringID m_name;
        uint32 m_size;
        uint32 m_binding;
        ShaderUniforms m_uniforms;
    };

    using ShaderUniformBlocks = std::vector<ShaderUniformBlock>;
}