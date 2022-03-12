#include "rex_events_pch.h"
#include "input/mouse_event.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        MouseEvent::MouseEvent(const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : InputEvent(windowID, timeStamp, nativeEvent)
            , m_mouse_info(mouseInfo)
        {
        }

        //-------------------------------------------------------------------------
        MouseEvent::~MouseEvent() = default;

        //-------------------------------------------------------------------------
        bool MouseEvent::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        const rex::MousePosition& MouseEvent::get_screen_position() const
        {
            return m_mouse_info.screen_position;
        }

        //-------------------------------------------------------------------------
        const rex::MousePosition& MouseEvent::get_local_position() const
        {
            return m_mouse_info.local_position;
        }
    }
}