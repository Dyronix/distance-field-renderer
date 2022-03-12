#include "rex_graphics_pch.h"
#include "renderer/types/primitive_type.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const PrimitiveType& e)
    {
        switch (e)
        {
            case PrimitiveType::NONE: os << "PrimitiveType: NONE"; break;
            case PrimitiveType::POINTS: os << "PrimitiveType: POINTS"; break;

            case PrimitiveType::LINES: os << "PrimitiveType: LINES"; break;
            case PrimitiveType::LINE_STRIP: os << "PrimitiveType: LINE_STRIP"; break;
            case PrimitiveType::LINE_LOOP: os << "PrimitiveType: LINE_LOOP"; break;

            case PrimitiveType::TRIANGLES: os << "PrimitiveType: TRIANGLES"; break;
            case PrimitiveType::TRIANGLE_STRIP: os << "PrimitiveType: TRIANGLE_STRIP"; break;
            case PrimitiveType::TRIANGLE_FAN: os << "PrimitiveType: TRIANGLE_FAN"; break;

            case PrimitiveType::QUADS: os << "PrimitiveType: QUADS"; break;
        }

        return os;
    }
}