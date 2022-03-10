#pragma once

#include "memory/refptr.h"

#include "resources/apiresource.h"
#include "resources/shaderuniformblock.h"

namespace sbt
{
    class UniformBuffer : public Resource
    {
    public:
        UniformBuffer() = default;
        virtual ~UniformBuffer() = default;

        virtual void        setData(const void* data, uint32 size, uint32 offset = 0, IsRenderThread rt = IsRenderThread::NO) = 0;

        virtual StringID    getName() const = 0;

        virtual uint32      getBinding() const = 0;
        virtual uint32      getSize() const = 0;

        virtual const ShaderUniforms& getUniforms() const = 0;
    };
}