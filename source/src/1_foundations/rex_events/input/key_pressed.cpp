#include "rex_events_pch.h"
#include "input/key_pressed.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        KeyPressed::KeyPressed(const int32 key, const int16 modifiers, const int8 repeatCount, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : KeyEvent(key, windowID, timeStamp, nativeEvent)
            , m_modifiers(modifiers)
            , m_repeat_count(repeatCount)
        {
            R_TRACE("Key pressed event created");
        }
        //-------------------------------------------------------------------------
        KeyPressed::~KeyPressed() = default;

        //-------------------------------------------------------------------------
        const int16 KeyPressed::get_modifiers() const
        {
            return m_modifiers;
        }
        //-------------------------------------------------------------------------
        const int16 KeyPressed::get_repeat_count() const
        {
            return m_repeat_count;
        }

        //-------------------------------------------------------------------------
        std::string KeyPressed::to_string() const
        {
            std::stringstream s;
            s << "Key Pressed: key: " << get_key();
            return s.str();
        }
    }
}