#include "rex_events_pch.h"
#include "window/window_maximized.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowMaximized::WindowMaximized(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window maximized event created");
        }
        //-------------------------------------------------------------------------
        WindowMaximized::~WindowMaximized() = default;

        //-------------------------------------------------------------------------
        bool WindowMaximized::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowMaximized::to_string() const
        {
            std::stringstream s;
            s << "Window Maximized";
            return s.str();
        }
    }
}