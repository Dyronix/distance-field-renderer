#pragma once

#include "input/mouse_event.h"

namespace rex
{
    struct MouseDelta
    {
        int32 x;
        int32 y;
    };

    namespace events
    {
        class MouseMove : public MouseEvent
        {
        public:
            EVENT_CLASS_TYPE(MouseMove);

            MouseMove(const MouseKeyMetaData& keyMetaData, const MouseInfo& mouseInfo, const MouseDelta& mouseDelta, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);
            ~MouseMove() override;

            const MouseDelta& get_mouse_delta() const;

            bool is_ctrl_down() const;
            bool is_shift_down() const;

            std::string to_string() const override;

        private:
            const MouseDelta m_mouse_delta;

            bool m_is_ctrl_down;
            bool m_is_shift_down;
        };
    }
}