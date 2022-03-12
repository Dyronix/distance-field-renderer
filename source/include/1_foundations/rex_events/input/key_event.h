#pragma once

#include "input/input_event.h"

namespace rex
{
    namespace events
    {
        class KeyEvent : public InputEvent
        {
        public:
            EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::KEYBOARD);

            ~KeyEvent() override;

            bool can_consume() const override;

            const int32 get_key() const;

        protected:
            KeyEvent(const int32 key, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);

        private:
            const int32 m_key;
        };
    }
}