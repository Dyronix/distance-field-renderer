#include "rex_events_pch.h"
#include "window/window_gained_keyboard_focus.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowGainedKeyboardFocus::WindowGainedKeyboardFocus(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window gained keyboard focus event created");
        }
        //-------------------------------------------------------------------------
        WindowGainedKeyboardFocus::~WindowGainedKeyboardFocus() = default;

        //-------------------------------------------------------------------------
        bool WindowGainedKeyboardFocus::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowGainedKeyboardFocus::to_string() const
        {
            std::stringstream s;
            s << "Window gained Keyboard Focus";
            return s.str();
        }
    }
}