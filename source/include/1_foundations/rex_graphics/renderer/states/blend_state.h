#pragma once

#include "yesnoenum.h"
#include "renderer/types/blend_function.h"

namespace rex
{
    using BlendEnabled = YesNoEnum;

    struct BlendState
    {
        BlendState()
            : enabled(BlendEnabled::YES)
            , src(BlendFunction::SRC_ALPHA)
            , dst(BlendFunction::ONE_MINUS_SRC_ALPHA)
        {
        }

        BlendEnabled enabled;

        BlendFunction src;
        BlendFunction dst;
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const BlendState& e);
}