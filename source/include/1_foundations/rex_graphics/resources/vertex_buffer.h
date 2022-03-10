#pragma once

#include "memory/refptr.h"

#include "resources/apiresource.h"
#include "resources/bufferlayout.h"

namespace sbt
{
    using Layout = BufferLayout;

    class VertexBuffer : public Resource
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void release() = 0;

        virtual uint32 getCount() = 0;
        virtual const uint32 getCount() const = 0;

        virtual Layout& getLayout() = 0;
        virtual const Layout& getLayout() const = 0;

        virtual void setLayout(const Layout& layout) = 0;

    protected:
        VertexBuffer() = default;
    };
}