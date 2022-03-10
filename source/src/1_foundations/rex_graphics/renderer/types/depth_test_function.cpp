#include "rex_graphics_pch.h"
#include "renderer/types/depth_test_function.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const DepthTestFunction& e)
    {
        switch (e)
        {
            case DepthTestFunction::NONE: os << "Depth Test fn: NONE"; break;
            case DepthTestFunction::ALWAYS: os << "Depth Test fn: ALWAYS"; break;
            case DepthTestFunction::EQUAL: os << "Depth Test fn: EQUAL"; break;
            case DepthTestFunction::GREATER: os << "Depth Test fn: GREATER"; break;
            case DepthTestFunction::GREATER_OR_EQUAL: os << "Depth Test fn: GREATER OR EQUAL"; break;
            case DepthTestFunction::LESS: os << "Depth Test fn: LESS"; break;
            case DepthTestFunction::LESS_OR_EQUAL: os << "Depth Test fn: LESS OR EQUAL"; break;
            case DepthTestFunction::NEVER: os << "Depth Test fn: NEVER"; break;
            case DepthTestFunction::NOT_EQUAL: os << "Depth Test fn: NOT EQUAL"; break;
        }

        return os;
    }
}