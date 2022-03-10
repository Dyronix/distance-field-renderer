#include "rex_graphics_pch.h"
#include "renderer/types/blend_function.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const BlendFunction& e)
    {
        switch (e)
        {
            case BlendFunction::ZERO: return os << "Blend Function: ZERO"; break;
            case BlendFunction::ONE: return os << "Blend Function: ONE"; break;
            case BlendFunction::SRC_COLOR: return os << "Blend Function: SRC_COLOR"; break;
            case BlendFunction::ONE_MINUS_SRC_COLOR: return os << "Blend Function: ONE_MINUS_SRC_COLOR"; break;
            case BlendFunction::DST_COLOR: return os << "Blend Function: DST_COLOR"; break;
            case BlendFunction::ONE_MINUS_DST_COLOR: return os << "Blend Function: ONE_MINUS_DST_COLOR"; break;
            case BlendFunction::SRC_ALPHA: return os << "Blend Function: SRC_ALPHA"; break;
            case BlendFunction::ONE_MINUS_SRC_ALPHA: return os << "Blend Function: ONE_MINUS_SRC_ALPHA"; break;
            case BlendFunction::DST_ALPHA: return os << "Blend Function: DST_ALPHA"; break;
            case BlendFunction::ONE_MINUS_DST_ALPHA: return os << "Blend Function: ONE_MINUS_DST_ALPHA"; break;
            case BlendFunction::CONSTANT_COLOR: return os << "Blend Function: CONSTANT_COLOR"; break;
            case BlendFunction::ONE_MINUS_CONSTANT_COLOR: return os << "Blend Function: ONE_MINUS_CONSTANT_COLOR"; break;
            case BlendFunction::CONSTANT_ALPHA: return os << "Blend Function: CONSTANT_ALPHA"; break;
            case BlendFunction::ONE_MINUS_CONSTANT_ALPHA: return os << "Blend Function: ONE_MINUS_CONSTANT_ALPHA"; break;
        }

        return os;
    }
}