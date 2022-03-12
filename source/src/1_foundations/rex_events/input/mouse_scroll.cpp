#include "rex_events_pch.h"
#include "input/mouse_scroll.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        MouseScroll::MouseScroll(const MouseKeyMetaData& keyMetaData, const ScrollData& scrollData, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : MouseEvent(mouseInfo, windowID, timeStamp, nativeEvent)
            , m_scroll_data(scrollData)
            , m_is_ctrl_down(keyMetaData.is_ctrl_down)
            , m_is_shift_down(keyMetaData.is_shift_down)
        {
            R_TRACE("Mouse scroll event created");
        }
        //-------------------------------------------------------------------------
        MouseScroll::~MouseScroll() = default;

        //-------------------------------------------------------------------------
        bool MouseScroll::is_ctrl_down() const
        {
            return m_is_ctrl_down;
        }

        //-------------------------------------------------------------------------
        bool MouseScroll::is_shift_down() const
        {
            return m_is_shift_down;
        }

        //-------------------------------------------------------------------------
        rex::ScrollDirection MouseScroll::get_scroll_direction(ScrollAxis axis) const
        {
            return m_scroll_data.at(axis).direction;
        }
        //-------------------------------------------------------------------------
        float MouseScroll::get_scroll_delta(ScrollAxis axis) const
        {
            return m_scroll_data.at(axis).wheel_delta;
        }

        //-------------------------------------------------------------------------
        std::string MouseScroll::to_string() const
        {
            std::stringstream s;
            s << "Mouse Scrolled: [ xDelta: " << m_scroll_data.at(ScrollAxis::X_AXIS).wheel_delta << ", yDelta: " << m_scroll_data.at(ScrollAxis::Y_AXIS).wheel_delta << "] [ xPos: " << get_screen_position().x << ", yPos: " << get_screen_position().y
              << "]";
            return s.str();
        }
    }
}