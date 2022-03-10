#include "rex_graphics_pch.h"

#include "renderer/states/face_cull_state.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const FaceCullState& e)
    {
        if (e.enabled)
        {
            os << "Face Culling Enabled: TRUE";
        }
        else
        {
            os << "Face Culling Enabled: FALSE";
        }

        os << "\t" << e.cull_mode;
        os << "\t" << e.face_winding;

        return os;
    }
}