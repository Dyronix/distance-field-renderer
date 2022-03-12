#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowMaximized : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowMaximized);

            WindowMaximized(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowMaximized() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
