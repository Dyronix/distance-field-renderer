#include "rex_windows_pch.h"

#include "input/mouse/win32_mouse_codes.h"

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
        {
            switch (mouseCode)
            {
                case MouseCode::BUTTON_UNKNOWN: os << mousenames::MOUSE_BUTTON_UNKNOWN; break;
                case MouseCode::BUTTON_LEFT: os << mousenames::MOUSE_BUTTON_LEFT; break;
                case MouseCode::BUTTON_MIDDLE: os << mousenames::MOUSE_BUTTON_MIDDLE; break;
                case MouseCode::BUTTON_RIGHT: os << mousenames::MOUSE_BUTTON_RIGHT; break;
                case MouseCode::BUTTON_X1: os << mousenames::MOUSE_BUTTON_X1; break;
                case MouseCode::BUTTON_X2: os << mousenames::MOUSE_BUTTON_X2; break;
            }

            return os;
        }
    }
}