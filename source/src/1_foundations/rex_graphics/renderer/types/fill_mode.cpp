#include "rex_graphics_pch.h"
#include "renderer/types/fill_mode.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const FillMode& e)
    {
        switch (e)
        {
        case FillMode::FILL:    os << "FillMode: FILL"; break;
        case FillMode::LINE:    os << "FillMode: LINE"; break;
        case FillMode::POINT:   os << "FillMode: POINT"; break;
        }

        return os;
    }
}