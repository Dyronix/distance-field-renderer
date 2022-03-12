#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowLostKeyboardFocus : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowLostKeyboardFocus);

            WindowLostKeyboardFocus(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowLostKeyboardFocus() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
