#pragma once

#include "yesnoenum.h"
#include "renderer/types/depth_test_function.h"

namespace rex
{
    using DepthTestEnabled = YesNoEnum;
    using DepthBufferReadOnly = YesNoEnum;

    struct DepthTestState
    {
        DepthTestState()
            : enabled(DepthTestEnabled::YES)
            , function(DepthTestFunction::LESS)
            , read_only(DepthBufferReadOnly::NO)
        {
        }
        DepthTestState(DepthTestEnabled enabled)
            : enabled(enabled)
            , function(enabled ? DepthTestFunction::LESS : DepthTestFunction::NONE)
            , read_only(DepthBufferReadOnly::NO)
        {
        }
        DepthTestState(DepthTestEnabled enabled, DepthTestFunction func, DepthBufferReadOnly readonly)
            : enabled(enabled)
            , function(func)
            , read_only(readonly)
        {
        }

        DepthTestEnabled enabled;
        DepthTestFunction function;

        DepthBufferReadOnly read_only;
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const DepthTestState& e);
}