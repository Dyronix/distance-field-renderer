#pragma once

#include "mouse/mouse_names.h"

#include <SDL_mouse.h>

namespace rex
{
    namespace win32
    {
        enum class MouseCode
        {
            BUTTON_UNKNOWN = -1,
            BUTTON_LEFT = SDL_BUTTON_LEFT,
            BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
            BUTTON_RIGHT = SDL_BUTTON_RIGHT,
            BUTTON_X1 = SDL_BUTTON_X1,
            BUTTON_X2 = SDL_BUTTON_X2,
        };

        std::ostream& operator<<(std::ostream& os, MouseCode mouseCode);
    }
}

#define R_MOUSE_BUTTON_UNKNOWN (int16) rex::win32::MouseCode::BUTTON_UNKNOWN
#define R_MOUSE_BUTTON_LEFT (int16) rex::win32::MouseCode::BUTTON_LEFT
#define R_MOUSE_BUTTON_MIDDLE (int16) rex::win32::MouseCode::BUTTON_MIDDLE
#define R_MOUSE_BUTTON_RIGHT (int16) rex::win32::MouseCode::BUTTON_RIGHT
#define R_MOUSE_BUTTON_X1 (int16) rex::win32::MouseCode::BUTTON_X1
#define R_MOUSE_BUTTON_X2 (int16) rex::win32::MouseCode::BUTTON_X2