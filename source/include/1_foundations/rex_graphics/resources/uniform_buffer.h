#pragma once

#include "resources/api_resource.h"
#include "resources/shader_uniform.h"

namespace rex
{
    class UniformBuffer : public Resource
    {
    public:
        UniformBuffer() = default;
        virtual ~UniformBuffer() = default;

        virtual void set_data(const void* data, uint32 size, uint32 offset = 0, IsRenderThread rt = IsRenderThread::NO) = 0;

        virtual StringID get_name() const = 0;

        virtual uint32 get_binding() const = 0;
        virtual uint32 get_size() const = 0;

        virtual const ShaderUniforms& get_uniforms() const = 0;
    };
}