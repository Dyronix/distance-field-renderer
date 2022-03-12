#pragma once

#include "blob.h"

#include "resources/api_resource.h"

#include "resources/shader_uniform.h"
#include "resources/shader_attribute.h"
#include "resources/shader_resource.h"
#include "resources/shader_uniform_block.h"

namespace rex
{
    class ShaderUniformLocation;
    class ShaderResourceLocation;

    struct ShaderProgramCreationInfo
    {
        StringID tag;
        StringID queue;

        memory::Blob vertexShaderCode;
        memory::Blob fragmentShaderCode;

        ShaderAttributes attributes;
        ShaderUniforms uniforms;
        ShaderResources resources;
        ShaderUniformBlocks blocks;
    };

    class ShaderProgram : public Resource
    {
    public:
        ShaderProgram() = default;
        virtual ~ShaderProgram() = default;

        virtual bool is_bound() const = 0;
        virtual bool is_linked() const = 0;

        virtual const StringID& get_name() const = 0;

        virtual uint32 get_queue() const = 0;
        virtual uint32 get_shader_program_id() const = 0;

        virtual const ShaderUniform& get_uniform(const StringID& name) const = 0;
        virtual const ShaderUniformLocation& get_uniform_location(const StringID& name) const = 0;
        virtual const ShaderUniforms get_uniforms() const = 0;
        virtual const int32 get_uniform_count() const = 0;

        virtual const ShaderAttribute& get_attribute(const StringID& name) const = 0;
        virtual const ShaderAttributes get_attributes() const = 0;
        virtual const int32 get_attribute_count() const = 0;

        virtual const ShaderResource& get_resource(const StringID& name) const = 0;
        virtual const ShaderResourceLocation& get_resource_location(const StringID& name) const = 0;
        virtual const ShaderResources get_resources() const = 0;
        virtual const int32 get_resource_count() const = 0;

        virtual const ShaderUniformBlock& get_uniform_block(const StringID& name) const = 0;
        virtual const ShaderUniformBlocks get_uniform_blocks() const = 0;
        virtual const int32 get_uniform_block_count() const = 0;

        virtual void bind(IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void unbind(IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void set_uniform(const StringID& name, const rex::matrix3& mat, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const rex::matrix4& mat, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void set_uniform(const StringID& name, const float value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const rex::vec2& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const rex::vec3& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const rex::vec4& value, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void set_uniform(const StringID& name, const rex::vec2i& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const rex::vec3i& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const rex::vec4i& value, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void set_uniform(const StringID& name, const float* values, uint32 size, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void set_uniform(const StringID& name, const int32 value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const uint32 value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void set_uniform(const StringID& name, const bool value, IsRenderThread rt = IsRenderThread::NO) const = 0;
    };
}