#pragma once

#include "yesnoenum.h"
#include "renderer/types/cull_mode.h"
#include "renderer/types/face_winding.h"

namespace rex
{
    using FaceCullingEnabled = YesNoEnum;
    using FaceCullingCullMode = CullMode;

    struct FaceCullState
    {
        FaceCullState()
            : enabled(FaceCullingEnabled::YES)
            , cull_mode(FaceCullingCullMode::BACK)
            , face_winding(FaceWinding::COUNTER_CLOCKWISE)
        {
        }

        FaceCullState(FaceCullingEnabled enabled)
            : enabled(enabled)
            , cull_mode(enabled ? FaceCullingCullMode::BACK : FaceCullingCullMode::NONE)
            , face_winding(FaceWinding::COUNTER_CLOCKWISE)
        {
        }

        FaceCullState(FaceCullingEnabled enabled, FaceCullingCullMode cullmode, FaceWinding winding)
            : enabled(enabled)
            , cull_mode(enabled ? cullmode : FaceCullingCullMode::NONE)
            , face_winding(winding)
        {
        }

        FaceCullingEnabled enabled;
        FaceCullingCullMode cull_mode;
        FaceWinding face_winding;
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const FaceCullState& e);
}