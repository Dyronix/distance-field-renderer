#include "rex_graphics_pch.h"

#include "resources/shader_uniform.h"

namespace rex
{
    //-------------------------------------------------------------------------
    rex::ShaderUniform ShaderUniform::EMPTY = {};

    //-------------------------------------------------------------------------
    ShaderUniform::ShaderUniform()
        : m_name(ESID::SID_None)
        , m_type(ShaderUniformType::NONE)
        , m_size(0)
        , m_offset(0)
    {
    }
    //-------------------------------------------------------------------------
    ShaderUniform::ShaderUniform(const StringID& name, const ShaderUniformType& type, uint32 size, uint32 offset)
        : m_name(name)
        , m_type(type)
        , m_size(size)
        , m_offset(offset)
    {
    }

    //-------------------------------------------------------------------------
    const rex::StringID& ShaderUniform::get_name() const
    {
        return m_name;
    }
    //-------------------------------------------------------------------------
    const rex::ShaderUniformType& ShaderUniform::get_type() const
    {
        return m_type;
    }
    //-------------------------------------------------------------------------
    uint32 ShaderUniform::get_size() const
    {
        return m_size;
    }
    //-------------------------------------------------------------------------
    uint32 ShaderUniform::get_offset() const
    {
        return m_offset;
    }

    //-------------------------------------------------------------------------
    bool ShaderUniform::is_valid() const
    {
        return this->m_name != EMPTY.get_name() || this->m_type != EMPTY.get_type() || this->m_size != EMPTY.get_size() || this->get_offset() != EMPTY.get_offset();
    }
}