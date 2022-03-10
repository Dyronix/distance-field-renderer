#pragma once

namespace rex
{
    enum class FaceWinding
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const FaceWinding& e);
}