#pragma once

#include "input/mouse_button_event.h"

namespace rex
{
    namespace events
    {
        class MouseButtonPressed : public MouseButtonEvent
        {
        public:
            EVENT_CLASS_TYPE(MouseButtonPressed);

            MouseButtonPressed(const MouseKeyMetaData& keyMetaData, const uint16 button, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);
            ~MouseButtonPressed() override;

            bool is_ctrl_down() const;
            bool is_shift_down() const;

            std::string to_string() const override;

        private:
            bool m_is_ctrl_down;
            bool m_is_shift_down;
        };
    }
}