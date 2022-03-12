#include "rex_events_pch.h"
#include "window/window_lost_mouse_focus.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowLostMouseFocus::WindowLostMouseFocus(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window lost mouse focus event created");
        }
        //-------------------------------------------------------------------------
        WindowLostMouseFocus::~WindowLostMouseFocus() = default;

        //-------------------------------------------------------------------------
        bool WindowLostMouseFocus::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowLostMouseFocus::to_string() const
        {
            std::stringstream s;
            s << "Window lost Mouse Focus";
            return s.str();
        }
    }
}