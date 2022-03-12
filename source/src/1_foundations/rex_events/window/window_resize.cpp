#include "rex_events_pch.h"
#include "window/window_resize.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        WindowResize::WindowResize(uint32 width, uint32 height, uint32 windowID, const memory::Blob& nativeEvent)
            : WindowEvent(windowID, nativeEvent)
            , m_width(width)
            , m_height(height)
        {
            R_TRACE("Window resize event created");
        }

        WindowResize::~WindowResize()
        {
        }

        //-------------------------------------------------------------------------
        bool WindowResize::can_consume() const
        {
            return true;
        }

        //-------------------------------------------------------------------------
        uint32 WindowResize::get_width() const
        {
            return m_width;
        }
        //-------------------------------------------------------------------------
        uint32 WindowResize::get_height() const
        {
            return m_height;
        }

        //-------------------------------------------------------------------------
        std::string WindowResize::to_string() const
        {
            std::stringstream s;
            s << "Window Resized: [" << get_width() << "," << get_height() << "]";
            return s.str();
        }
    }
}