#include "rex_graphics_pch.h"

#include "resources/shader_attribute.h"

namespace rex
{
    //-------------------------------------------------------------------------
    rex::ShaderAttribute ShaderAttribute::EMPTY = {};

    //-------------------------------------------------------------------------
    ShaderAttribute::ShaderAttribute()
        :m_name(ESID::SID_None)
        , m_type(ShaderAttributeType::NONE)
        , m_size(0)
        , m_offset(0)
    {

    }

    //-------------------------------------------------------------------------
    ShaderAttribute::ShaderAttribute(const StringID& name, ShaderAttributeType type, uint32 size, uint32 offset)
        :m_name(name)
        , m_type(type)
        , m_size(size)
        , m_offset(offset)
    {

    }

    //-------------------------------------------------------------------------
    ShaderAttribute::~ShaderAttribute()
    {

    }

    //-------------------------------------------------------------------------
    const rex::StringID& ShaderAttribute::get_name() const
    {
        return m_name;
    }

    //-------------------------------------------------------------------------
    const rex::ShaderAttributeType& ShaderAttribute::get_type() const
    {
        return m_type;
    }

    //-------------------------------------------------------------------------
    uint32 ShaderAttribute::get_size() const
    {
        return m_size;
    }

    //-------------------------------------------------------------------------
    uint32 ShaderAttribute::get_offset() const
    {
        return m_offset;
    }

    //-------------------------------------------------------------------------
    bool ShaderAttribute::is_valid() const
    {
        return this->m_name != EMPTY.get_name() || this->m_type != EMPTY.get_type() || this->m_size != EMPTY.get_size() || this->get_offset() != EMPTY.get_offset();
    }
}