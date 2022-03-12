#pragma once

namespace rex
{
    template <typename T>
    inline T lerp(T a, T b, T t)
    {
        return (T(1.0f) - t) * a + b * t;
    }
}