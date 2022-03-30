#pragma once

#include "resources/api_resource.h"

namespace rex
{
    class IndexBuffer : public Resource
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32 get_count() = 0;
        virtual const uint32 get_count() const = 0;

        virtual uint32 get_triangle_count() = 0;
        virtual const uint32 get_triangle_count() const = 0;

    protected:
        IndexBuffer() = default;
    };
}