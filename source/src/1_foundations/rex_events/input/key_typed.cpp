#include "rex_events_pch.h"
#include "input/key_typed.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        KeyTyped::KeyTyped(const int8* text, const uint64 windowID, const uint32 timestamp, const memory::Blob& nativeEvent)
            : InputEvent(windowID, timestamp, nativeEvent)
            , m_text(text)
        {
            R_TRACE("Key typed event created");
        }
        //-------------------------------------------------------------------------
        KeyTyped::~KeyTyped() = default;

        //-------------------------------------------------------------------------
        bool KeyTyped::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        const int8* KeyTyped::get_text() const
        {
            return m_text;
        }

        //-------------------------------------------------------------------------
        std::string KeyTyped::to_string() const
        {
            std::stringstream ss;

            size_t size = (sizeof(m_text) / sizeof((m_text)[0]));

            ss << "Key Typed:";

            for (int32 i = 0; i < size; ++i)
            {
                ss << m_text[i];
            }

            return ss.str();
        }
    }
}