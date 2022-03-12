#pragma once

namespace rex
{
    template <typename T>
    inline T remap(T value, T from1, T to1, T from2, T to2)
    {
        return (value - from1) / (from2 - from1) * (to2 - to1) + to1;
    }
}