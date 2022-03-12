#pragma once

#include "input/mouse_event.h"

namespace rex
{
    namespace events
    {
        class MouseButtonEvent : public MouseEvent
        {
        public:
            ~MouseButtonEvent() override;

            const uint16 get_button() const;

        protected:
            MouseButtonEvent(const uint16 button, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);

        private:
            const uint16 m_button;
        };
    }
}