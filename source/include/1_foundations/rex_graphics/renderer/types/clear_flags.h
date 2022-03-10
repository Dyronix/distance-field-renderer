#pragma once

#include "defines.h"

namespace rex
{
    enum class ClearFlags
    {
        NONE = 0,
        COLOR = BIT(0),
        DEPTH = BIT(1),
        STENCIL = BIT(2)
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const ClearFlags& e);

    static ClearFlags CLEAR_NONE = ClearFlags::NONE;
    static ClearFlags COLOR_ONLY = ClearFlags::COLOR;
    static ClearFlags DEPTH_ONLY = ClearFlags::DEPTH;
    static ClearFlags STENCIL_ONLY = ClearFlags::STENCIL;
    static ClearFlags CLEAR_COLOR_AND_DEPTH = (ClearFlags)((int)ClearFlags::COLOR | (int)ClearFlags::DEPTH);
    static ClearFlags CLEAR_ALL = (ClearFlags)((int)ClearFlags::COLOR | (int)ClearFlags::DEPTH | (int)ClearFlags::STENCIL);
}