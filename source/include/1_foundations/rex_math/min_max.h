#pragma once

#include "vec3.h"

namespace rex
{
    template <typename T>
    struct MinMax
    {
        MinMax()
            : minimum(T())
            , maximum(T())
        {
        }
        MinMax(const T& inMin, const T& inMax)
            : minimum(std::min(inMin, inMax))
            , maximum(std::max(inMin, inMax))
        {
        }

        T minimum;
        T maximum;
    };

    using MinMaxI = MinMax<int32>;
    using MinMaxF = MinMax<float>;

    using AABB = MinMax<rex::vec3>;
}