#pragma once

#ifdef DELETE
#undef DELETE
#endif

namespace rex
{
    namespace keynames
    {
        constexpr const char* KEY_UNKNOWN = "UNKNOWN";

        /* Printable keys */
        constexpr const char* KEY_SPACE = "SPACE";
        constexpr const char* KEY_APOSTROPHE = "APOSTROPHE";
        constexpr const char* KEY_COMMA = "COMMA";
        constexpr const char* KEY_MINUS = "MINUS";
        constexpr const char* KEY_PERIOD = "PERIOD";
        constexpr const char* KEY_SLASH = "SLASH";
        constexpr const char* KEY_0 = "0";
        constexpr const char* KEY_1 = "1";
        constexpr const char* KEY_2 = "2";
        constexpr const char* KEY_3 = "3";
        constexpr const char* KEY_4 = "4";
        constexpr const char* KEY_5 = "5";
        constexpr const char* KEY_6 = "6";
        constexpr const char* KEY_7 = "7";
        constexpr const char* KEY_8 = "8";
        constexpr const char* KEY_9 = "9";
        constexpr const char* KEY_SEMICOLON = "SEMICOLON";
        constexpr const char* KEY_EQUAL = "EQUAL";
        constexpr const char* KEY_A = "A";
        constexpr const char* KEY_B = "B";
        constexpr const char* KEY_C = "C";
        constexpr const char* KEY_D = "D";
        constexpr const char* KEY_E = "E";
        constexpr const char* KEY_F = "F";
        constexpr const char* KEY_G = "G";
        constexpr const char* KEY_H = "H";
        constexpr const char* KEY_I = "I";
        constexpr const char* KEY_J = "J";
        constexpr const char* KEY_K = "K";
        constexpr const char* KEY_L = "L";
        constexpr const char* KEY_M = "M";
        constexpr const char* KEY_N = "N";
        constexpr const char* KEY_O = "O";
        constexpr const char* KEY_P = "P";
        constexpr const char* KEY_Q = "Q";
        constexpr const char* KEY_R = "R";
        constexpr const char* KEY_S = "S";
        constexpr const char* KEY_T = "T";
        constexpr const char* KEY_U = "U";
        constexpr const char* KEY_V = "V";
        constexpr const char* KEY_W = "W";
        constexpr const char* KEY_X = "X";
        constexpr const char* KEY_Y = "Y";
        constexpr const char* KEY_Z = "Z";
        constexpr const char* KEY_LEFT_BRACKET = "LEFT_BRACKET";
        constexpr const char* KEY_BACKSLASH = "BACKSLASH";
        constexpr const char* KEY_RIGHT_BRACKET = "RIGHT_BRACKET";
        constexpr const char* KEY_GRAVE_ACCENT = "GRAVE_ACCENT";
        constexpr const char* KEY_WORLD_1 = "WORLD_1";
        constexpr const char* KEY_WORLD_2 = "WORLD_2";

        /* Function keys */
        constexpr const char* KEY_ESCAPE = "ESCAPE";
        constexpr const char* KEY_ENTER = "ENTER";
        constexpr const char* KEY_TAB = "TAB";
        constexpr const char* KEY_BACKSPACE = "BACKSPACE";
        constexpr const char* KEY_INSERT = "INSERT";
        constexpr const char* KEY_DELETE = "DELETE";
        constexpr const char* KEY_RIGHT = "RIGHT";
        constexpr const char* KEY_LEFT = "LEFT";
        constexpr const char* KEY_DOWN = "DOWN";
        constexpr const char* KEY_UP = "UP";
        constexpr const char* KEY_PAGE_UP = "PAGE_UP";
        constexpr const char* KEY_PAGE_DOWN = "PAGE_DOWN";
        constexpr const char* KEY_HOME = "HOME";
        constexpr const char* KEY_END = "END";
        constexpr const char* KEY_CAPS_LOCK = "CAPS_LOCK";
        constexpr const char* KEY_SCROLL_LOCK = "SCROLL_LOCK";
        constexpr const char* KEY_NUM_LOCK = "NUM_LOCK";
        constexpr const char* KEY_PRINT_SCREEN = "PRINT_SCREEN";
        constexpr const char* KEY_PAUSE = "PAUSE";
        constexpr const char* KEY_F1 = "F1";
        constexpr const char* KEY_F2 = "F2";
        constexpr const char* KEY_F3 = "F3";
        constexpr const char* KEY_F4 = "F4";
        constexpr const char* KEY_F5 = "F5";
        constexpr const char* KEY_F6 = "F6";
        constexpr const char* KEY_F7 = "F7";
        constexpr const char* KEY_F8 = "F8";
        constexpr const char* KEY_F9 = "F9";
        constexpr const char* KEY_F10 = "F10";
        constexpr const char* KEY_F11 = "F11";
        constexpr const char* KEY_F12 = "F12";
        constexpr const char* KEY_F13 = "F13";
        constexpr const char* KEY_F14 = "F14";
        constexpr const char* KEY_F15 = "F15";
        constexpr const char* KEY_F16 = "F16";
        constexpr const char* KEY_F17 = "F17";
        constexpr const char* KEY_F18 = "F18";
        constexpr const char* KEY_F19 = "F19";
        constexpr const char* KEY_F20 = "F20";
        constexpr const char* KEY_F21 = "F21";
        constexpr const char* KEY_F22 = "F22";
        constexpr const char* KEY_F23 = "F23";
        constexpr const char* KEY_F24 = "F24";
        constexpr const char* KEY_F25 = "F25";
        constexpr const char* KEY_KP_0 = "KP_0";
        constexpr const char* KEY_KP_1 = "KP_1";
        constexpr const char* KEY_KP_2 = "KP_2";
        constexpr const char* KEY_KP_3 = "KP_3";
        constexpr const char* KEY_KP_4 = "KP_4";
        constexpr const char* KEY_KP_5 = "KP_5";
        constexpr const char* KEY_KP_6 = "KP_6";
        constexpr const char* KEY_KP_7 = "KP_7";
        constexpr const char* KEY_KP_8 = "KP_8";
        constexpr const char* KEY_KP_9 = "KP_9";
        constexpr const char* KEY_KP_DECIMAL = "KP_DECIMAL";
        constexpr const char* KEY_KP_DIVIDE = "KP_DIVIDE";
        constexpr const char* KEY_KP_MULTIPLY = "KP_MULTIPLY";
        constexpr const char* KEY_KP_SUBTRACT = "KP_SUBTRACT";
        constexpr const char* KEY_KP_ADD = "KP_ADD";
        constexpr const char* KEY_KP_ENTER = "KP_ENTER";
        constexpr const char* KEY_KP_EQUAL = "KP_EQUAL";
        constexpr const char* KEY_LEFT_SHIFT = "LEFT_SHIFT";
        constexpr const char* KEY_LEFT_CONTROL = "LEFT_CONTROL";
        constexpr const char* KEY_LEFT_ALT = "LEFT_ALT";
        constexpr const char* KEY_LEFT_SUPER = "LEFT_SUPER";
        constexpr const char* KEY_RIGHT_SHIFT = "RIGHT_SHIFT";
        constexpr const char* KEY_RIGHT_CONTROL = "RIGHT_CONTROL";
        constexpr const char* KEY_RIGHT_ALT = "RIGHT_ALT";
        constexpr const char* KEY_RIGHT_SUPER = "RIGHT_SUPER";
        constexpr const char* KEY_MENU = "MENU";
    }
}