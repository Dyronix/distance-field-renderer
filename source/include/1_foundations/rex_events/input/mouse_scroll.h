#pragma once

#include "input/mouse_event.h"
#include "input/mouse_scroll_meta.h"

namespace rex
{
    namespace events
    {
        class MouseScroll : public MouseEvent
        {
        public:
            EVENT_CLASS_TYPE(MouseScroll);

            MouseScroll(const MouseKeyMetaData& keyMetaData, const ScrollData& scrollMetaData, const MouseInfo& mouseInfo, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);
            ~MouseScroll() override;

            bool is_ctrl_down() const;
            bool is_shift_down() const;

            ScrollDirection get_scroll_direction(ScrollAxis axis) const;
            float get_scroll_delta(ScrollAxis axis) const;

            std::string to_string() const override;

        private:
            bool m_is_ctrl_down;
            bool m_is_shift_down;

            ScrollData m_scroll_data;
        };
    }
}