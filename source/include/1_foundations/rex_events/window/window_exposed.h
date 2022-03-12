#pragma once

#include "window/window_event.h"

namespace rex
{
    namespace events
    {
        class WindowExposed : public WindowEvent
        {
        public:
            EVENT_CLASS_TYPE(WindowExposed);

            WindowExposed(uint32 windowID, const memory::Blob& nativeEvent);
            ~WindowExposed() override;

            bool can_consume() const override;

            std::string to_string() const override;
        };
    }
}
