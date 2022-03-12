#include "rex_events_pch.h"
#include "input/mouse_move.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        MouseMove::MouseMove(const MouseKeyMetaData& keyMetaData, const MouseInfo& mouseInfo, const MouseDelta& mouseDelta, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : MouseEvent(mouseInfo, windowID, timeStamp, nativeEvent)
            , m_mouse_delta(mouseDelta)
            , m_is_ctrl_down(keyMetaData.is_ctrl_down)
            , m_is_shift_down(keyMetaData.is_shift_down)
        {
            R_TRACE("Mouse move event created");
        }

        MouseMove::~MouseMove() = default;

        //-------------------------------------------------------------------------
        const MouseDelta& MouseMove::get_mouse_delta() const
        {
            return m_mouse_delta;
        }

        //-------------------------------------------------------------------------
        bool MouseMove::is_ctrl_down() const
        {
            return m_is_ctrl_down;
        }

        //-------------------------------------------------------------------------
        bool MouseMove::is_shift_down() const
        {
            return m_is_shift_down;
        }

        //-------------------------------------------------------------------------
        std::string MouseMove::to_string() const
        {
            std::stringstream s;
            s << "Mouse Moved: [" << get_local_position().x << "," << get_local_position().y << "]";
            return s.str();
        }
    }
}