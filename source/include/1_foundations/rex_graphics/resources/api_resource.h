#pragma once

#include "memory/ref_counted_object.h"

namespace rex
{
    class Resource : public RefCountedObject
    {
    public:
        virtual ~Resource() = default;

        virtual void release() = 0;
    };
}