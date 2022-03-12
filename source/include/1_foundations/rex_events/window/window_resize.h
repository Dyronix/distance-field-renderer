#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowResize : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowResize);

            WindowResize(uint32 width, uint32 height, uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowResize() override;

            bool can_consume() const override;

            uint32 get_width() const;
            uint32 get_height() const;

            std::string to_string() const override;

        private:
            uint32 m_width;
            uint32 m_height;
        };
    }
}