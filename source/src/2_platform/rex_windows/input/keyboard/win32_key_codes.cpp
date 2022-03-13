#include "rex_windows_pch.h"

#include "input/keyboard/win32_key_codes.h"

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
        {
            switch (keyCode)
            {
                /* The unknown key */
                case KeyCode::KEY_UNKNOWN:
                    os << keynames::KEY_UNKNOWN;
                    break;

                /* Printable keys */
                case KeyCode::KEY_SPACE: os << keynames::KEY_SPACE; break;
                case KeyCode::KEY_APOSTROPHE: os << keynames::KEY_APOSTROPHE; break;
                case KeyCode::KEY_COMMA: os << keynames::KEY_COMMA; break;
                case KeyCode::KEY_MINUS: os << keynames::KEY_MINUS; break;
                case KeyCode::KEY_PERIOD: os << keynames::KEY_PERIOD; break;
                case KeyCode::KEY_SLASH: os << keynames::KEY_SLASH; break;
                case KeyCode::KEY_0: os << keynames::KEY_0; break;
                case KeyCode::KEY_1: os << keynames::KEY_1; break;
                case KeyCode::KEY_2: os << keynames::KEY_2; break;
                case KeyCode::KEY_3: os << keynames::KEY_3; break;
                case KeyCode::KEY_4: os << keynames::KEY_4; break;
                case KeyCode::KEY_5: os << keynames::KEY_5; break;
                case KeyCode::KEY_6: os << keynames::KEY_6; break;
                case KeyCode::KEY_7: os << keynames::KEY_7; break;
                case KeyCode::KEY_8: os << keynames::KEY_8; break;
                case KeyCode::KEY_9: os << keynames::KEY_9; break;
                case KeyCode::KEY_SEMICOLON: os << keynames::KEY_SEMICOLON; break;
                case KeyCode::KEY_EQUAL: os << keynames::KEY_EQUAL; break;
                case KeyCode::KEY_A: os << keynames::KEY_A; break;
                case KeyCode::KEY_B: os << keynames::KEY_B; break;
                case KeyCode::KEY_C: os << keynames::KEY_C; break;
                case KeyCode::KEY_D: os << keynames::KEY_D; break;
                case KeyCode::KEY_E: os << keynames::KEY_E; break;
                case KeyCode::KEY_F: os << keynames::KEY_F; break;
                case KeyCode::KEY_G: os << keynames::KEY_G; break;
                case KeyCode::KEY_H: os << keynames::KEY_H; break;
                case KeyCode::KEY_I: os << keynames::KEY_I; break;
                case KeyCode::KEY_J: os << keynames::KEY_J; break;
                case KeyCode::KEY_K: os << keynames::KEY_K; break;
                case KeyCode::KEY_L: os << keynames::KEY_L; break;
                case KeyCode::KEY_M: os << keynames::KEY_M; break;
                case KeyCode::KEY_N: os << keynames::KEY_N; break;
                case KeyCode::KEY_O: os << keynames::KEY_O; break;
                case KeyCode::KEY_P: os << keynames::KEY_P; break;
                case KeyCode::KEY_Q: os << keynames::KEY_Q; break;
                case KeyCode::KEY_R: os << keynames::KEY_R; break;
                case KeyCode::KEY_S: os << keynames::KEY_S; break;
                case KeyCode::KEY_T: os << keynames::KEY_T; break;
                case KeyCode::KEY_U: os << keynames::KEY_U; break;
                case KeyCode::KEY_V: os << keynames::KEY_V; break;
                case KeyCode::KEY_W: os << keynames::KEY_W; break;
                case KeyCode::KEY_X: os << keynames::KEY_X; break;
                case KeyCode::KEY_Y: os << keynames::KEY_Y; break;
                case KeyCode::KEY_Z: os << keynames::KEY_Z; break;
                case KeyCode::KEY_LEFT_BRACKET: os << keynames::KEY_LEFT_BRACKET; break;
                case KeyCode::KEY_BACKSLASH: os << keynames::KEY_BACKSLASH; break;
                case KeyCode::KEY_RIGHT_BRACKET: os << keynames::KEY_RIGHT_BRACKET; break;
                case KeyCode::KEY_GRAVE_ACCENT:
                    os << keynames::KEY_GRAVE_ACCENT;
                    break;

                /* Function keys */
                case KeyCode::KEY_ESCAPE: os << keynames::KEY_ESCAPE; break;
                case KeyCode::KEY_ENTER: os << keynames::KEY_ENTER; break;
                case KeyCode::KEY_TAB: os << keynames::KEY_TAB; break;
                case KeyCode::KEY_BACKSPACE: os << keynames::KEY_BACKSPACE; break;
                case KeyCode::KEY_INSERT: os << keynames::KEY_INSERT; break;
                case KeyCode::KEY_DELETE: os << keynames::KEY_DELETE; break;
                case KeyCode::KEY_RIGHT: os << keynames::KEY_RIGHT; break;
                case KeyCode::KEY_LEFT: os << keynames::KEY_LEFT; break;
                case KeyCode::KEY_DOWN: os << keynames::KEY_DOWN; break;
                case KeyCode::KEY_UP: os << keynames::KEY_UP; break;
                case KeyCode::KEY_PAGE_UP: os << keynames::KEY_PAGE_UP; break;
                case KeyCode::KEY_PAGE_DOWN: os << keynames::KEY_PAGE_DOWN; break;
                case KeyCode::KEY_HOME: os << keynames::KEY_HOME; break;
                case KeyCode::KEY_END: os << keynames::KEY_END; break;
                case KeyCode::KEY_CAPS_LOCK: os << keynames::KEY_CAPS_LOCK; break;
                case KeyCode::KEY_SCROLL_LOCK: os << keynames::KEY_SCROLL_LOCK; break;
                case KeyCode::KEY_NUM_LOCK: os << keynames::KEY_NUM_LOCK; break;
                case KeyCode::KEY_PRINT_SCREEN: os << keynames::KEY_PRINT_SCREEN; break;
                case KeyCode::KEY_PAUSE: os << keynames::KEY_PAUSE; break;
                case KeyCode::KEY_F1: os << keynames::KEY_F1; break;
                case KeyCode::KEY_F2: os << keynames::KEY_F2; break;
                case KeyCode::KEY_F3: os << keynames::KEY_F3; break;
                case KeyCode::KEY_F4: os << keynames::KEY_F4; break;
                case KeyCode::KEY_F5: os << keynames::KEY_F5; break;
                case KeyCode::KEY_F6: os << keynames::KEY_F6; break;
                case KeyCode::KEY_F7: os << keynames::KEY_F7; break;
                case KeyCode::KEY_F8: os << keynames::KEY_F8; break;
                case KeyCode::KEY_F9: os << keynames::KEY_F9; break;
                case KeyCode::KEY_F10: os << keynames::KEY_F10; break;
                case KeyCode::KEY_F11: os << keynames::KEY_F11; break;
                case KeyCode::KEY_F12: os << keynames::KEY_F12; break;
                case KeyCode::KEY_F13: os << keynames::KEY_F13; break;
                case KeyCode::KEY_F14: os << keynames::KEY_F14; break;
                case KeyCode::KEY_F15: os << keynames::KEY_F15; break;
                case KeyCode::KEY_F16: os << keynames::KEY_F16; break;
                case KeyCode::KEY_F17: os << keynames::KEY_F17; break;
                case KeyCode::KEY_F18: os << keynames::KEY_F18; break;
                case KeyCode::KEY_F19: os << keynames::KEY_F19; break;
                case KeyCode::KEY_F20: os << keynames::KEY_F20; break;
                case KeyCode::KEY_F21: os << keynames::KEY_F21; break;
                case KeyCode::KEY_F22: os << keynames::KEY_F22; break;
                case KeyCode::KEY_F23: os << keynames::KEY_F23; break;
                case KeyCode::KEY_F24: os << keynames::KEY_F24; break;
                case KeyCode::KEY_KP_0: os << keynames::KEY_KP_0; break;
                case KeyCode::KEY_KP_1: os << keynames::KEY_KP_1; break;
                case KeyCode::KEY_KP_2: os << keynames::KEY_KP_2; break;
                case KeyCode::KEY_KP_3: os << keynames::KEY_KP_3; break;
                case KeyCode::KEY_KP_4: os << keynames::KEY_KP_4; break;
                case KeyCode::KEY_KP_5: os << keynames::KEY_KP_5; break;
                case KeyCode::KEY_KP_6: os << keynames::KEY_KP_6; break;
                case KeyCode::KEY_KP_7: os << keynames::KEY_KP_7; break;
                case KeyCode::KEY_KP_8: os << keynames::KEY_KP_8; break;
                case KeyCode::KEY_KP_9: os << keynames::KEY_KP_9; break;
                case KeyCode::KEY_KP_DECIMAL: os << keynames::KEY_KP_DECIMAL; break;
                case KeyCode::KEY_KP_DIVIDE: os << keynames::KEY_KP_DIVIDE; break;
                case KeyCode::KEY_KP_MULTIPLY: os << keynames::KEY_KP_MULTIPLY; break;
                case KeyCode::KEY_KP_SUBTRACT: os << keynames::KEY_KP_SUBTRACT; break;
                case KeyCode::KEY_KP_ADD: os << keynames::KEY_KP_ADD; break;
                case KeyCode::KEY_KP_ENTER: os << keynames::KEY_KP_ENTER; break;
                case KeyCode::KEY_KP_EQUAL: os << keynames::KEY_KP_EQUAL; break;
                case KeyCode::KEY_LEFT_SHIFT: os << keynames::KEY_LEFT_SHIFT; break;
                case KeyCode::KEY_LEFT_CONTROL: os << keynames::KEY_LEFT_CONTROL; break;
                case KeyCode::KEY_LEFT_ALT: os << keynames::KEY_LEFT_ALT; break;
                case KeyCode::KEY_RIGHT_SHIFT: os << keynames::KEY_RIGHT_SHIFT; break;
                case KeyCode::KEY_RIGHT_CONTROL: os << keynames::KEY_RIGHT_CONTROL; break;
                case KeyCode::KEY_RIGHT_ALT: os << keynames::KEY_RIGHT_ALT; break;
                case KeyCode::KEY_MENU: os << keynames::KEY_MENU; break;
            }

            return os;
        }
    }
}