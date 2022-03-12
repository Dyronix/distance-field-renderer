#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowLostMouseFocus : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowLostMouseFocus);

            WindowLostMouseFocus(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowLostMouseFocus() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
