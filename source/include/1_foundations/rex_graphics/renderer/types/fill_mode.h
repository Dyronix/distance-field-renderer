#pragma once

namespace rex
{
    enum class FillMode
    {
        POINT,
        LINE,
        FILL
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const FillMode& e);
}