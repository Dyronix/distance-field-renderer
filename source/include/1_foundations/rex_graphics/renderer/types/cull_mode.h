#pragma once

namespace rex
{
    enum class CullMode
    {
        NONE,
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const CullMode& e);
}