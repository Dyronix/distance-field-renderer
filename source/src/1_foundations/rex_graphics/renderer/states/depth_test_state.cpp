#include "rex_graphics_pch.h"

#include "renderer/states/depth_test_state.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const DepthTestState& e)
    {
        if (e.enabled)
        {
            os << "\t" << "Depth Testing Enabled: TRUE";
        }
        else
        {
            os << "\t" << "Depth Testing Enabled: FALSE";
        }

        os << "\t" << e.function;

        if (e.enabled)
        {
            os << "\t" << "Depth Buffer Read Only: TRUE";
        }
        else
        {
            os << "\t" << "Depth Buffer Read Only: FALSE";
        }

        return os;
    }
}