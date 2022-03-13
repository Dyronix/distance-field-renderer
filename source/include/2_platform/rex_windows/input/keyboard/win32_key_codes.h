#pragma once

#include "keyboard/key_names.h"

#include <SDL_keyboard.h>
#include <SDL_scancode.h>

#if defined rex_PLATFORM_WINDOWS
#undef DELETE
#endif

namespace rex
{
    namespace win32
    {
        enum class KeyCode
        {
            /* The unknown key */
            KEY_UNKNOWN = SDL_SCANCODE_UNKNOWN,

            /* Printable keys */
            KEY_SPACE = SDL_SCANCODE_SPACE,
            KEY_APOSTROPHE = SDL_SCANCODE_APOSTROPHE, /* ' */
            KEY_COMMA = SDL_SCANCODE_COMMA,           /* , */
            KEY_MINUS = SDL_SCANCODE_MINUS,           /* - */
            KEY_PERIOD = SDL_SCANCODE_PERIOD,         /* . */
            KEY_SLASH = SDL_SCANCODE_SLASH,           /* / */
            KEY_0 = SDL_SCANCODE_1,
            KEY_1 = SDL_SCANCODE_2,
            KEY_2 = SDL_SCANCODE_3,
            KEY_3 = SDL_SCANCODE_4,
            KEY_4 = SDL_SCANCODE_5,
            KEY_5 = SDL_SCANCODE_6,
            KEY_6 = SDL_SCANCODE_7,
            KEY_7 = SDL_SCANCODE_8,
            KEY_8 = SDL_SCANCODE_9,
            KEY_9 = SDL_SCANCODE_0,
            KEY_SEMICOLON = SDL_SCANCODE_SEMICOLON, /* ; */
            KEY_EQUAL = SDL_SCANCODE_EQUALS,        /* = */
            KEY_A = SDL_SCANCODE_A,
            KEY_B = SDL_SCANCODE_B,
            KEY_C = SDL_SCANCODE_C,
            KEY_D = SDL_SCANCODE_D,
            KEY_E = SDL_SCANCODE_E,
            KEY_F = SDL_SCANCODE_F,
            KEY_G = SDL_SCANCODE_G,
            KEY_H = SDL_SCANCODE_H,
            KEY_I = SDL_SCANCODE_I,
            KEY_J = SDL_SCANCODE_J,
            KEY_K = SDL_SCANCODE_K,
            KEY_L = SDL_SCANCODE_L,
            KEY_M = SDL_SCANCODE_M,
            KEY_N = SDL_SCANCODE_N,
            KEY_O = SDL_SCANCODE_O,
            KEY_P = SDL_SCANCODE_P,
            KEY_Q = SDL_SCANCODE_Q,
            KEY_R = SDL_SCANCODE_R,
            KEY_S = SDL_SCANCODE_S,
            KEY_T = SDL_SCANCODE_T,
            KEY_U = SDL_SCANCODE_U,
            KEY_V = SDL_SCANCODE_V,
            KEY_W = SDL_SCANCODE_W,
            KEY_X = SDL_SCANCODE_X,
            KEY_Y = SDL_SCANCODE_Y,
            KEY_Z = SDL_SCANCODE_Z,
            KEY_LEFT_BRACKET = SDL_SCANCODE_LEFTBRACKET,   /* [ */
            KEY_BACKSLASH = SDL_SCANCODE_BACKSLASH,        /* \ */
            KEY_RIGHT_BRACKET = SDL_SCANCODE_RIGHTBRACKET, /* ] */
            KEY_GRAVE_ACCENT = SDL_SCANCODE_GRAVE,         /* ` */

            /* Function keys */
            KEY_ESCAPE = SDL_SCANCODE_ESCAPE,
            KEY_ENTER = SDL_SCANCODE_RETURN,
            KEY_TAB = SDL_SCANCODE_TAB,
            KEY_BACKSPACE = SDL_SCANCODE_BACKSPACE,
            KEY_INSERT = SDL_SCANCODE_INSERT,
            KEY_DELETE = SDL_SCANCODE_DELETE,
            KEY_RIGHT = SDL_SCANCODE_RIGHT,
            KEY_LEFT = SDL_SCANCODE_LEFT,
            KEY_DOWN = SDL_SCANCODE_DOWN,
            KEY_UP = SDL_SCANCODE_UP,
            KEY_PAGE_UP = SDL_SCANCODE_PAGEUP,
            KEY_PAGE_DOWN = SDL_SCANCODE_PAGEDOWN,
            KEY_HOME = SDL_SCANCODE_HOME,
            KEY_END = SDL_SCANCODE_END,
            KEY_CAPS_LOCK = SDL_SCANCODE_CAPSLOCK,
            KEY_SCROLL_LOCK = SDL_SCANCODE_SCROLLLOCK,
            KEY_NUM_LOCK = SDL_SCANCODE_NUMLOCKCLEAR,
            KEY_PRINT_SCREEN = SDL_SCANCODE_PRINTSCREEN,
            KEY_PAUSE = SDL_SCANCODE_PAUSE,
            KEY_F1 = SDL_SCANCODE_F1,
            KEY_F2 = SDL_SCANCODE_F2,
            KEY_F3 = SDL_SCANCODE_F3,
            KEY_F4 = SDL_SCANCODE_F4,
            KEY_F5 = SDL_SCANCODE_F5,
            KEY_F6 = SDL_SCANCODE_F6,
            KEY_F7 = SDL_SCANCODE_F7,
            KEY_F8 = SDL_SCANCODE_F8,
            KEY_F9 = SDL_SCANCODE_F9,
            KEY_F10 = SDL_SCANCODE_F10,
            KEY_F11 = SDL_SCANCODE_F11,
            KEY_F12 = SDL_SCANCODE_F12,
            KEY_F13 = SDL_SCANCODE_F13,
            KEY_F14 = SDL_SCANCODE_F14,
            KEY_F15 = SDL_SCANCODE_F15,
            KEY_F16 = SDL_SCANCODE_F16,
            KEY_F17 = SDL_SCANCODE_F17,
            KEY_F18 = SDL_SCANCODE_F18,
            KEY_F19 = SDL_SCANCODE_F19,
            KEY_F20 = SDL_SCANCODE_F20,
            KEY_F21 = SDL_SCANCODE_F21,
            KEY_F22 = SDL_SCANCODE_F22,
            KEY_F23 = SDL_SCANCODE_F23,
            KEY_F24 = SDL_SCANCODE_F24,
            KEY_KP_0 = SDL_SCANCODE_KP_0,
            KEY_KP_1 = SDL_SCANCODE_KP_1,
            KEY_KP_2 = SDL_SCANCODE_KP_2,
            KEY_KP_3 = SDL_SCANCODE_KP_3,
            KEY_KP_4 = SDL_SCANCODE_KP_4,
            KEY_KP_5 = SDL_SCANCODE_KP_5,
            KEY_KP_6 = SDL_SCANCODE_KP_6,
            KEY_KP_7 = SDL_SCANCODE_KP_7,
            KEY_KP_8 = SDL_SCANCODE_KP_8,
            KEY_KP_9 = SDL_SCANCODE_KP_9,
            KEY_KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
            KEY_KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
            KEY_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
            KEY_KP_SUBTRACT = SDL_SCANCODE_KP_MINUS,
            KEY_KP_ADD = SDL_SCANCODE_KP_PLUS,
            KEY_KP_ENTER = SDL_SCANCODE_KP_ENTER,
            KEY_KP_EQUAL = SDL_SCANCODE_KP_EQUALS,
            KEY_LEFT_SHIFT = SDL_SCANCODE_LSHIFT,
            KEY_LEFT_CONTROL = SDL_SCANCODE_LCTRL,
            KEY_LEFT_ALT = SDL_SCANCODE_LALT,
            KEY_RIGHT_SHIFT = SDL_SCANCODE_RSHIFT,
            KEY_RIGHT_CONTROL = SDL_SCANCODE_RCTRL,
            KEY_RIGHT_ALT = SDL_SCANCODE_RALT,
            KEY_MENU = SDL_SCANCODE_MENU
        };

        std::ostream& operator<<(std::ostream& os, KeyCode keyCode);
    }
}

/* The unknown key */
#define R_KEY_UNKNOWN (int16) rex::win32::KeyCode::KEY_UNKNOWN

/* Printable keys */
#define R_KEY_SPACE (int16) rex::win32::KeyCode::KEY_SPACE
#define R_KEY_APOSTROPHE (int16) rex::win32::KeyCode::KEY_APOSTROPHE
#define R_KEY_COMMA (int16) rex::win32::KeyCode::KEY_COMMA
#define R_KEY_MINUS (int16) rex::win32::KeyCode::KEY_MINUS
#define R_KEY_PERIOD (int16) rex::win32::KeyCode::KEY_PERIOD
#define R_KEY_SLASH (int16) rex::win32::KeyCode::KEY_SLASH
#define R_KEY_0 (int16) rex::win32::KeyCode::KEY_0
#define R_KEY_1 (int16) rex::win32::KeyCode::KEY_1
#define R_KEY_2 (int16) rex::win32::KeyCode::KEY_2
#define R_KEY_3 (int16) rex::win32::KeyCode::KEY_3
#define R_KEY_4 (int16) rex::win32::KeyCode::KEY_4
#define R_KEY_5 (int16) rex::win32::KeyCode::KEY_5
#define R_KEY_6 (int16) rex::win32::KeyCode::KEY_6
#define R_KEY_7 (int16) rex::win32::KeyCode::KEY_7
#define R_KEY_8 (int16) rex::win32::KeyCode::KEY_8
#define R_KEY_9 (int16) rex::win32::KeyCode::KEY_9
#define R_KEY_SEMICOLON (int16) rex::win32::KeyCode::KEY_SEMICOLON
#define R_KEY_EQUAL (int16) rex::win32::KeyCode::KEY_EQUAL
#define R_KEY_A (int16) rex::win32::KeyCode::KEY_A
#define R_KEY_B (int16) rex::win32::KeyCode::KEY_B
#define R_KEY_C (int16) rex::win32::KeyCode::KEY_C
#define R_KEY_D (int16) rex::win32::KeyCode::KEY_D
#define R_KEY_E (int16) rex::win32::KeyCode::KEY_E
#define R_KEY_F (int16) rex::win32::KeyCode::KEY_F
#define R_KEY_G (int16) rex::win32::KeyCode::KEY_G
#define R_KEY_H (int16) rex::win32::KeyCode::KEY_H
#define R_KEY_I (int16) rex::win32::KeyCode::KEY_I
#define R_KEY_J (int16) rex::win32::KeyCode::KEY_J
#define R_KEY_K (int16) rex::win32::KeyCode::KEY_K
#define R_KEY_L (int16) rex::win32::KeyCode::KEY_L
#define R_KEY_M (int16) rex::win32::KeyCode::KEY_M
#define R_KEY_N (int16) rex::win32::KeyCode::KEY_N
#define R_KEY_O (int16) rex::win32::KeyCode::KEY_O
#define R_KEY_P (int16) rex::win32::KeyCode::KEY_P
#define R_KEY_Q (int16) rex::win32::KeyCode::KEY_Q
#define R_KEY_R (int16) rex::win32::KeyCode::KEY_R
#define R_KEY_S (int16) rex::win32::KeyCode::KEY_S
#define R_KEY_T (int16) rex::win32::KeyCode::KEY_T
#define R_KEY_U (int16) rex::win32::KeyCode::KEY_U
#define R_KEY_V (int16) rex::win32::KeyCode::KEY_V
#define R_KEY_W (int16) rex::win32::KeyCode::KEY_W
#define R_KEY_X (int16) rex::win32::KeyCode::KEY_X
#define R_KEY_Y (int16) rex::win32::KeyCode::KEY_Y
#define R_KEY_Z (int16) rex::win32::KeyCode::KEY_Z
#define R_KEY_LEFT_BRACKET (int16) rex::win32::KeyCode::KEY_LEFT_BRACKET
#define R_KEY_BACKSLASH (int16) rex::win32::KeyCode::KEY_BACKSLASH
#define R_KEY_RIGHT_BRACKET (int16) rex::win32::KeyCode::KEY_RIGHT_BRACKET
#define R_KEY_GRAVE_ACCENT (int16) rex::win32::KeyCode::KEY_GRAVE_ACCENT

/* Function keys */
#define R_KEY_ESCAPE (int16) rex::win32::KeyCode::KEY_ESCAPE
#define R_KEY_ENTER (int16) rex::win32::KeyCode::KEY_ENTER
#define R_KEY_TAB (int16) rex::win32::KeyCode::KEY_TAB
#define R_KEY_BACKSPACE (int16) rex::win32::KeyCode::KEY_BACKSPACE
#define R_KEY_INSERT (int16) rex::win32::KeyCode::KEY_INSERT
#define R_KEY_DELETE (int16) rex::win32::KeyCode::KEY_DELETE
#define R_KEY_RIGHT (int16) rex::win32::KeyCode::KEY_RIGHT
#define R_KEY_LEFT (int16) rex::win32::KeyCode::KEY_LEFT
#define R_KEY_DOWN (int16) rex::win32::KeyCode::KEY_DOWN
#define R_KEY_UP (int16) rex::win32::KeyCode::KEY_UP
#define R_KEY_PAGE_UP (int16) rex::win32::KeyCode::KEY_PAGE_UP
#define R_KEY_PAGE_DOWN (int16) rex::win32::KeyCode::KEY_PAGE_DOWN
#define R_KEY_HOME (int16) rex::win32::KeyCode::KEY_HOME
#define R_KEY_END (int16) rex::win32::KeyCode::KEY_END
#define R_KEY_CAPS_LOCK (int16) rex::win32::KeyCode::KEY_CAPS_LOCK
#define R_KEY_SCROLL_LOCK (int16) rex::win32::KeyCode::KEY_SCROLL_LOCK
#define R_KEY_NUM_LOCK (int16) rex::win32::KeyCode::KEY_NUM_LOCK
#define R_KEY_PRINT_SCREEN (int16) rex::win32::KeyCode::KEY_PRINT_SCREEN
#define R_KEY_PAUSE (int16) rex::win32::KeyCode::KEY_PAUSE
#define R_KEY_F1 (int16) rex::win32::KeyCode::KEY_F1
#define R_KEY_F2 (int16) rex::win32::KeyCode::KEY_F2
#define R_KEY_F3 (int16) rex::win32::KeyCode::KEY_F3
#define R_KEY_F4 (int16) rex::win32::KeyCode::KEY_F4
#define R_KEY_F5 (int16) rex::win32::KeyCode::KEY_F5
#define R_KEY_F6 (int16) rex::win32::KeyCode::KEY_F6
#define R_KEY_F7 (int16) rex::win32::KeyCode::KEY_F7
#define R_KEY_F8 (int16) rex::win32::KeyCode::KEY_F8
#define R_KEY_F9 (int16) rex::win32::KeyCode::KEY_F9
#define R_KEY_F10 (int16) rex::win32::KeyCode::KEY_F10
#define R_KEY_F11 (int16) rex::win32::KeyCode::KEY_F11
#define R_KEY_F12 (int16) rex::win32::KeyCode::KEY_F12
#define R_KEY_F13 (int16) rex::win32::KeyCode::KEY_F13
#define R_KEY_F14 (int16) rex::win32::KeyCode::KEY_F14
#define R_KEY_F15 (int16) rex::win32::KeyCode::KEY_F15
#define R_KEY_F16 (int16) rex::win32::KeyCode::KEY_F16
#define R_KEY_F17 (int16) rex::win32::KeyCode::KEY_F17
#define R_KEY_F18 (int16) rex::win32::KeyCode::KEY_F18
#define R_KEY_F19 (int16) rex::win32::KeyCode::KEY_F19
#define R_KEY_F20 (int16) rex::win32::KeyCode::KEY_F20
#define R_KEY_F21 (int16) rex::win32::KeyCode::KEY_F21
#define R_KEY_F22 (int16) rex::win32::KeyCode::KEY_F22
#define R_KEY_F23 (int16) rex::win32::KeyCode::KEY_F23
#define R_KEY_F24 (int16) rex::win32::KeyCode::KEY_F24
#define R_KEY_KP_0 (int16) rex::win32::KeyCode::KEY_KP_0
#define R_KEY_KP_1 (int16) rex::win32::KeyCode::KEY_KP_1
#define R_KEY_KP_2 (int16) rex::win32::KeyCode::KEY_KP_2
#define R_KEY_KP_3 (int16) rex::win32::KeyCode::KEY_KP_3
#define R_KEY_KP_4 (int16) rex::win32::KeyCode::KEY_KP_4
#define R_KEY_KP_5 (int16) rex::win32::KeyCode::KEY_KP_5
#define R_KEY_KP_6 (int16) rex::win32::KeyCode::KEY_KP_6
#define R_KEY_KP_7 (int16) rex::win32::KeyCode::KEY_KP_7
#define R_KEY_KP_8 (int16) rex::win32::KeyCode::KEY_KP_8
#define R_KEY_KP_9 (int16) rex::win32::KeyCode::KEY_KP_9
#define R_KEY_KP_DECIMAL (int16) rex::win32::KeyCode::KEY_KP_DECIMAL
#define R_KEY_KP_DIVIDE (int16) rex::win32::KeyCode::KEY_KP_DIVIDE
#define R_KEY_KP_MULTIPLY (int16) rex::win32::KeyCode::KEY_KP_MULTIPLY
#define R_KEY_KP_SUBTRACT (int16) rex::win32::KeyCode::KEY_KP_SUBTRACT
#define R_KEY_KP_ADD (int16) rex::win32::KeyCode::KEY_KP_ADD
#define R_KEY_KP_ENTER (int16) rex::win32::KeyCode::KEY_KP_ENTER
#define R_KEY_KP_EQUAL (int16) rex::win32::KeyCode::KEY_KP_EQUAL
#define R_KEY_LEFT_SHIFT (int16) rex::win32::KeyCode::KEY_LEFT_SHIFT
#define R_KEY_LEFT_CONTROL (int16) rex::win32::KeyCode::KEY_LEFT_CONTROL
#define R_KEY_LEFT_ALT (int16) rex::win32::KeyCode::KEY_LEFT_ALT
#define R_KEY_RIGHT_SHIFT (int16) rex::win32::KeyCode::KEY_RIGHT_SHIFT
#define R_KEY_RIGHT_CONTROL (int16) rex::win32::KeyCode::KEY_RIGHT_CONTROL
#define R_KEY_RIGHT_ALT (int16) rex::win32::KeyCode::KEY_RIGHT_ALT
#define R_KEY_MENU (int16) rex::win32::KeyCode::KEY_MENU