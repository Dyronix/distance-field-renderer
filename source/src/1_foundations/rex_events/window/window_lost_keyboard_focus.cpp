#include "rex_events_pch.h"
#include "window/window_lost_keyboard_focus.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowLostKeyboardFocus::WindowLostKeyboardFocus(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window lost keyboard focus event created");
        }
        //-------------------------------------------------------------------------
        WindowLostKeyboardFocus::~WindowLostKeyboardFocus() = default;

        //-------------------------------------------------------------------------
        bool WindowLostKeyboardFocus::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowLostKeyboardFocus::to_string() const
        {
            std::stringstream s;
            s << "Window lost Keyboard Focus";
            return s.str();
        }
    }
}