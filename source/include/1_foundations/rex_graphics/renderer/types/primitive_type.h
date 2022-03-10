#pragma once

namespace rex
{
    enum class PrimitiveType
    {
        NONE,
        POINTS,

        LINES,
        LINE_STRIP,
        LINE_LOOP,

        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,

        QUADS,
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const PrimitiveType& e);
}