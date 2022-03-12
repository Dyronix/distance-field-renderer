#pragma once

#include <unordered_map>

namespace rex
{
    enum class ScrollAxis
    {
        X_AXIS,
        Y_AXIS
    };

    enum class ScrollDirection
    {
        AWAY_FROM_USER,
        TOWARDS_USER,
        LEFT,
        RIGHT
    };

    struct ScrollMetaData
    {
        ScrollDirection direction;
        float wheel_delta;
    };

    using ScrollData = std::unordered_map<ScrollAxis, ScrollMetaData>;
}