#include "rex_windows_pch.h"

#include "input/keyboard/win32_key_modifiers.h"

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        std::ostream& operator<<(std::ostream& os, ModifierCode modCode)
        {
            switch (modCode)
            {
                case ModifierCode::MOD_NONE: os << key_modifiers::KEYMOD_NONE; break;
                case ModifierCode::MOD_LSHIFT: os << key_modifiers::KEYMOD_LSHIFT; break;
                case ModifierCode::MOD_RSHIFT: os << key_modifiers::KEYMOD_RSHIFT; break;
                case ModifierCode::MOD_LCTRL: os << key_modifiers::KEYMOD_LCTRL; break;
                case ModifierCode::MOD_RCTRL: os << key_modifiers::KEYMOD_RCTRL; break;
                case ModifierCode::MOD_LALT: os << key_modifiers::KEYMOD_LALT; break;
                case ModifierCode::MOD_RALT: os << key_modifiers::KEYMOD_RALT; break;
                case ModifierCode::MOD_LGUI: os << key_modifiers::KEYMOD_LGUI; break;
                case ModifierCode::MOD_RGUI: os << key_modifiers::KEYMOD_RGUI; break;
                case ModifierCode::MOD_NUM: os << key_modifiers::KEYMOD_NUM; break;
                case ModifierCode::MOD_CAPS: os << key_modifiers::KEYMOD_CAPS; break;
                case ModifierCode::MOD_MODE: os << key_modifiers::KEYMOD_MODE; break;
            }

            return os;
        }
    }
}