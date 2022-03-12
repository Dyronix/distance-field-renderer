#include "rex_events_pch.h"

#include "input/input_event.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        InputEvent::InputEvent(const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : BaseEvent(nativeEvent)
            , m_window_id(windowID)
            , m_time_stamp(timeStamp)
        {
        }
        //-------------------------------------------------------------------------
        InputEvent::~InputEvent() = default;

        //-------------------------------------------------------------------------
        const uint64 InputEvent::get_window_id() const
        {
            return m_window_id;
        }

        //-------------------------------------------------------------------------
        const uint32 InputEvent::get_time_stamp() const
        {
            return m_time_stamp;
        }
    }
}