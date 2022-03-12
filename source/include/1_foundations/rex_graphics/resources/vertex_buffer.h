#pragma once

#include "resources/api_resource.h"
#include "resources/buffer_layout.h"

namespace rex
{
    using Layout = BufferLayout;

    class VertexBuffer : public Resource
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void release() = 0;

        virtual uint32 get_count() = 0;
        virtual const uint32 get_count() const = 0;

        virtual Layout& get_layout() = 0;
        virtual const Layout& get_layout() const = 0;

        virtual void set_layout(const Layout& layout) = 0;

    protected:
        VertexBuffer() = default;
    };
}