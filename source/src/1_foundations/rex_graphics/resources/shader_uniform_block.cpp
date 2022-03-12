#include "rex_graphics_pch.h"

#include "resources/shader_uniform_block.h"

namespace rex
{
    //-------------------------------------------------------------------------
    rex::ShaderUniformBlock ShaderUniformBlock::EMPTY = {};

    //-------------------------------------------------------------------------
    ShaderUniformBlock::ShaderUniformBlock()
        :m_name(ESID::SID_None)
        ,m_size(0)
        ,m_binding(0)
        ,m_uniforms()
    {

    }

    //-------------------------------------------------------------------------
    ShaderUniformBlock::ShaderUniformBlock(const StringID& name, uint32 size, uint32 binding, const rex::ShaderUniforms& uniforms)
        :m_name(name)
        ,m_size(size)
        ,m_binding(binding)
        ,m_uniforms(uniforms)
    {

    }

    //-------------------------------------------------------------------------
    ShaderUniformBlock::~ShaderUniformBlock()
    {

    }

    //-------------------------------------------------------------------------
    const rex::StringID& ShaderUniformBlock::get_name() const
    {
        return m_name;
    }

    //-------------------------------------------------------------------------
    const rex::ShaderUniforms& ShaderUniformBlock::get_uniforms() const
    {
        return m_uniforms;
    }

    //-------------------------------------------------------------------------
    uint32 ShaderUniformBlock::get_size() const
    {
        return m_size;
    }

    //-------------------------------------------------------------------------
    uint32 ShaderUniformBlock::get_binding() const
    {
        return m_binding;
    }

    //-------------------------------------------------------------------------
    uint32 ShaderUniformBlock::get_uniform_count() const
    {
        return static_cast<uint32>(m_uniforms.size());
    }

    //-------------------------------------------------------------------------
    bool ShaderUniformBlock::is_valid() const
    {
        return this->m_name != EMPTY.get_name() || this->m_size != EMPTY.get_size() || this->m_binding != EMPTY.get_binding();
    }
}