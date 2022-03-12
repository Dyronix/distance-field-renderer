#include "rex_events_pch.h"
#include "window/window_exposed.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowExposed::WindowExposed(uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
        {
            R_TRACE("Window exposed event created");
        }
        //-------------------------------------------------------------------------
        WindowExposed::~WindowExposed() = default;

        //-------------------------------------------------------------------------
        bool WindowExposed::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        std::string WindowExposed::to_string() const
        {
            std::stringstream s;
            s << "Window Exposed";
            return s.str();
        }
    }
}