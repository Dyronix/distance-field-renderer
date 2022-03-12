#include "rex_events_pch.h"
#include "input/mouse_button_pressed.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        MouseButtonPressed::MouseButtonPressed(const MouseKeyMetaData& keyMetaData, const uint16 button, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : MouseButtonEvent(button, mouseInfo, windowID, timeStamp, nativeEvent)
            , m_is_ctrl_down(keyMetaData.is_ctrl_down)
            , m_is_shift_down(keyMetaData.is_shift_down)
        {
            R_TRACE("Mouse button pressed event created");
        }

        //-------------------------------------------------------------------------
        MouseButtonPressed::~MouseButtonPressed() = default;

        //-------------------------------------------------------------------------
        bool MouseButtonPressed::is_ctrl_down() const
        {
            return m_is_ctrl_down;
        }

        //-------------------------------------------------------------------------
        bool MouseButtonPressed::is_shift_down() const
        {
            return m_is_shift_down;
        }

        //-------------------------------------------------------------------------
        std::string MouseButtonPressed::to_string() const
        {
            std::stringstream s;
            s << "Mouse button pressed: " << get_button();
            return s.str();
        }
    }
}