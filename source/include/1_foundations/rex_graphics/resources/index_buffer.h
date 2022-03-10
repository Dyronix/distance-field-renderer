#pragma once

#include "memory/refptr.h"

#include "resources/apiresource.h"

namespace sbt
{
    class IndexBuffer : public Resource
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void            bind() const = 0;
        virtual void            unbind() const = 0;

        virtual void*           getIndices() = 0;
        virtual const void*     getIndices() const = 0;

        virtual uint32          getCount() = 0;
        virtual const uint32    getCount() const = 0;

        virtual uint32          getTriangleCount() = 0;
        virtual const uint32    getTriangleCount() const = 0;

    protected:
        IndexBuffer() = default;
    };
}