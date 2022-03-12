#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowGainedMouseFocus : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowGainedMouseFocus);

            WindowGainedMouseFocus(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowGainedMouseFocus() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
