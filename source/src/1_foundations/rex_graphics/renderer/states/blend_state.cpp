#include "rex_graphics_pch.h"

#include "renderer/states/blend_state.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const BlendState& e)
    {
        if(e.enabled)
        {
            os << "\t" << "Blending Enabled: TRUE";
        }
        else
        {
            os << "\t" << "Blending Enabled: FALSE";
        }

        os << "\t" << "Src fn: " << e.src;
        os << "\t" << "Dst fn: " << e.dst;

        return os;
    }
}