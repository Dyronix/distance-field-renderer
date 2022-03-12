#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowGainedKeyboardFocus : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowGainedKeyboardFocus);

            WindowGainedKeyboardFocus(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowGainedKeyboardFocus() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
