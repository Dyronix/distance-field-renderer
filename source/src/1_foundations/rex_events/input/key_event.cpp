#include "rex_events_pch.h"
#include "input/key_event.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        KeyEvent::KeyEvent(const int32 key, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : InputEvent(windowID, timeStamp, nativeEvent)
            , m_key(key)
        {
        }
        //-------------------------------------------------------------------------
        KeyEvent::~KeyEvent() = default;

        //-------------------------------------------------------------------------
        bool KeyEvent::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        const int32 KeyEvent::get_key() const
        {
            return m_key;
        }
    }
}