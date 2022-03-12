#pragma once

#include "input/mouse_button_event.h"

namespace rex
{
    namespace events
    {
        class MouseButtonReleased : public MouseButtonEvent
        {
        public:
            EVENT_CLASS_TYPE(MouseButtonReleased);

            MouseButtonReleased(const uint16 button, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);
            ~MouseButtonReleased() override;

            std::string to_string() const override;
        };
    }
}