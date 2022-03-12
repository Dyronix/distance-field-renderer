#include "rex_events_pch.h"
#include "window/window_minized.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowMinimized::WindowMinimized(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window minimized event created");
        }
        //-------------------------------------------------------------------------
        WindowMinimized::~WindowMinimized() = default;

        //-------------------------------------------------------------------------
        bool WindowMinimized::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowMinimized::to_string() const
        {
            std::stringstream s;
            s << "Window Minimized";
            return s.str();
        }
    }
}