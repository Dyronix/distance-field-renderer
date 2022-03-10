#include "rex_graphics_pch.h"

#include "renderer/types/cull_mode.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const CullMode& e)
    {
        switch (e)
        {
        case CullMode::NONE:            os << "CullMode: NONE"; break;
        case CullMode::BACK:            os << "CullMode: BACK"; break;
        case CullMode::FRONT:           os << "CullMode: FRONT"; break;
        case CullMode::FRONT_AND_BACK:  os << "CullMode: FRONT_AND_BACK"; break;
        }

        return os;
    }
}