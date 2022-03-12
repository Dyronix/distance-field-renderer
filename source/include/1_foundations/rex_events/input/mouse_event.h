#pragma once

#include "input/input_event.h"

namespace rex
{
    using MousePosition = Position<int32>;
    using MouseInfo = Info<int32>;

    struct MouseKeyMetaData
    {
        static const int NO_MODS = 0;

        bool is_ctrl_down;
        bool is_shift_down;
    };

    namespace events
    {
        class MouseEvent : public InputEvent
        {
        public:
            EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE);

            ~MouseEvent() override;

            bool can_consume() const override;

            const MousePosition& get_screen_position() const;
            const MousePosition& get_local_position() const;

        protected:
            MouseEvent(const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);

        private:
            MouseInfo m_mouse_info;
        };
    }
}