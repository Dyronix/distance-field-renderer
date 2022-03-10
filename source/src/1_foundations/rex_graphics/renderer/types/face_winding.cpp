#include "rex_graphics_pch.h"
#include "renderer/types/face_winding.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const FaceWinding& e)
    {
        switch (e)
        {
        case FaceWinding::CLOCKWISE:            os << "FaceWinding: CLOCKWISE"; break;
        case FaceWinding::COUNTER_CLOCKWISE:    os << "FaceWinding: COUNTER_CLOCKWISE"; break;
        }

        return os;
    }
}