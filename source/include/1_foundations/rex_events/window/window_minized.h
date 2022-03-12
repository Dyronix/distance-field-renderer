#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowMinimized : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowMinimized);

            WindowMinimized(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowMinimized() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
