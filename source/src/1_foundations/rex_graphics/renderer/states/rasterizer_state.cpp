#include "rex_graphics_pch.h"

#include "renderer/states/rasterizer_state.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const RasterizerState& e)
    {
        os << "\t" << e.cull_mode;
        os << "\t" << e.fill_mode;

        return os;
    }
}