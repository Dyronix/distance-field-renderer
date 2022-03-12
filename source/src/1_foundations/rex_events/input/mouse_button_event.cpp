#include "rex_events_pch.h"
#include "input/mouse_button_event.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        MouseButtonEvent::MouseButtonEvent(const uint16 button, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : MouseEvent(mouseInfo, windowID, timeStamp, nativeEvent)
            , m_button(button)
        {
        }

        //-------------------------------------------------------------------------
        MouseButtonEvent::~MouseButtonEvent() = default;

        //-------------------------------------------------------------------------
        const uint16 MouseButtonEvent::get_button() const
        {
            return m_button;
        }
    }
}