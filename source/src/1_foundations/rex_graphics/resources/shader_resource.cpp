#include "rex_graphics_pch.h"

#include "resources/shader_resource.h"

rex::ShaderResource rex::ShaderResource::EMPTY = {};

//-------------------------------------------------------------------------
rex::ShaderResource::ShaderResource()
    :m_name(ESID::SID_None)
    ,m_type(ShaderUniformType::NONE)
    ,m_count(0)
    ,m_offset(0)
    ,m_register(0)
{

}

//-------------------------------------------------------------------------
rex::ShaderResource::ShaderResource(const StringID& name, ShaderUniformType type, uint32 count, uint32 offset, uint32 reg)
    :m_name(name)
    ,m_type(type)
    ,m_count(count)
    ,m_offset(offset)
    ,m_register(reg)
{

}

//-------------------------------------------------------------------------
const rex::StringID& rex::ShaderResource::get_name() const
{
    return m_name;
}
//-------------------------------------------------------------------------
const rex::ShaderUniformType& rex::ShaderResource::get_type() const
{
    return m_type;
}
//-------------------------------------------------------------------------
const uint32 rex::ShaderResource::get_count() const
{
    return m_count;
}
//-------------------------------------------------------------------------
const uint32 rex::ShaderResource::get_offset() const
{
    return m_offset;
}
//-------------------------------------------------------------------------
const uint32 rex::ShaderResource::get_register() const
{
    return m_register;
}

//-------------------------------------------------------------------------
bool rex::ShaderResource::is_valid() const
{
    return this->m_name != EMPTY.get_name() || this->m_count != EMPTY.get_count() || this->m_offset != EMPTY.get_offset() || this->m_register != EMPTY.get_register();
}
