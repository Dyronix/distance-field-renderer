#pragma once

#include "keyboard/key_modifier_names.h"

#include <SDL_keyboard.h>

namespace rex
{
    namespace win32
    {
        enum class ModifierCode
        {
            MOD_NONE = SDL_Keymod::KMOD_NONE,
            MOD_LSHIFT = SDL_Keymod::KMOD_LSHIFT,
            MOD_RSHIFT = SDL_Keymod::KMOD_RSHIFT,
            MOD_LCTRL = SDL_Keymod::KMOD_LCTRL,
            MOD_RCTRL = SDL_Keymod::KMOD_RCTRL,
            MOD_LALT = SDL_Keymod::KMOD_LALT,
            MOD_RALT = SDL_Keymod::KMOD_RALT,
            MOD_LGUI = SDL_Keymod::KMOD_LGUI,
            MOD_RGUI = SDL_Keymod::KMOD_RGUI,
            MOD_NUM = SDL_Keymod::KMOD_NUM,
            MOD_CAPS = SDL_Keymod::KMOD_CAPS,
            MOD_MODE = SDL_Keymod::KMOD_MODE
        };

        std::ostream& operator<<(std::ostream& os, ModifierCode modCode);
    }
}

#define R_KEY_MOD_NONE (int32) rex::win32::ModifierCode::MOD_NONE
#define R_KEY_MOD_LSHIFT (int32) rex::win32::ModifierCode::MOD_LSHIFT
#define R_KEY_MOD_RSHIFT (int32) rex::win32::ModifierCode::MOD_RSHIFT
#define R_KEY_MOD_LCTRL (int32) rex::win32::ModifierCode::MOD_LCTRL
#define R_KEY_MOD_RCTRL (int32) rex::win32::ModifierCode::MOD_RCTRL
#define R_KEY_MOD_LALT (int32) rex::win32::ModifierCode::MOD_LALT
#define R_KEY_MOD_RALT (int32) rex::win32::ModifierCode::MOD_RALT
#define R_KEY_MOD_LGUI (int32) rex::win32::ModifierCode::MOD_LGUI
#define R_KEY_MOD_RGUI (int32) rex::win32::ModifierCode::MOD_RGUI
#define R_KEY_MOD_NUM (int32) rex::win32::ModifierCode::MOD_NUM
#define R_KEY_MOD_CAPS (int32) rex::win32::ModifierCode::MOD_CAPS
#define R_KEY_MOD_MODE (int32) rex::win32::ModifierCode::MOD_MODE

#define R_KEY_MOD_CTRL KMOD_CTRL
#define R_KEY_MOD_SHIFT KMOD_SHIFT
#define R_KEY_MOD_ALT KMOD_ALT
#define R_KEY_MOD_GUI KMOD_GUI
