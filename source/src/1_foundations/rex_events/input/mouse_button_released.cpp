#include "rex_events_pch.h"
#include "input/mouse_button_released.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        MouseButtonReleased::MouseButtonReleased(const uint16 button, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : MouseButtonEvent(button, mouseInfo, windowID, timeStamp, nativeEvent)
        {
            R_TRACE("Mouse button released event created");
        }

        //-------------------------------------------------------------------------
        MouseButtonReleased::~MouseButtonReleased() = default;

        //-------------------------------------------------------------------------
        std::string MouseButtonReleased::to_string() const
        {
            std::stringstream s;
            s << "Mouse button released: " << get_button();
            return s.str();
        }
    }
}