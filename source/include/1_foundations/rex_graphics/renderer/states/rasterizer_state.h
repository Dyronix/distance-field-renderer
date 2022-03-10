#pragma once

#include "renderer/types/cull_mode.h"
#include "renderer/types/fill_mode.h"

namespace rex
{
    struct RasterizerState
    {
        RasterizerState()
            :cull_mode(CullMode::FRONT_AND_BACK)
            ,fill_mode(FillMode::FILL)
        {}
        RasterizerState(FillMode fillMode)
            :cull_mode(CullMode::FRONT_AND_BACK)
            ,fill_mode(fillMode)
        {}

        CullMode cull_mode;
        FillMode fill_mode;
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const RasterizerState& e);
}