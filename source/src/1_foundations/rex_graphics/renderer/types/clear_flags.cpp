#include "rex_graphics_pch.h"
#include "renderer/types/clear_flags.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const ClearFlags& e)
    {
        bool clear_color = (int)e & (int)ClearFlags::COLOR;
        bool clear_depth = (int)e & (int)ClearFlags::DEPTH;

        if (clear_color && !clear_depth)
        {
            os << "Clearing Color";
        }
        else if (!clear_color && clear_depth)
        {
            os << "Clearing Depth";
        }
        else if(clear_color && clear_depth)
        {
            os << "Clearing Color and Clearing Depth";
        }
        else
        {
            os << "No Clearing";
        }

        return os;
    }
}