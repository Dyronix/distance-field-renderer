#pragma once

namespace rex
{
    template <typename T>
    inline T invlerp(float a, float b, float v)
    {
        assert(b - a != 0);

        return (v - a) / (b - a);
    }
}