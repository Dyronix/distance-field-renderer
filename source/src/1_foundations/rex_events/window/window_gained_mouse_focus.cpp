#include "rex_events_pch.h"
#include "window/window_gained_mouse_focus.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowGainedMouseFocus::WindowGainedMouseFocus(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window gained mouse focus event created");
        }
        //-------------------------------------------------------------------------
        WindowGainedMouseFocus::~WindowGainedMouseFocus() = default;

        //-------------------------------------------------------------------------
        bool WindowGainedMouseFocus::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowGainedMouseFocus::to_string() const
        {
            std::stringstream s;
            s << "Window gained Mouse Focus";
            return s.str();
        }
    }
}