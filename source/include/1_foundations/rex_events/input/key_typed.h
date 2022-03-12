#pragma once

#include "input/input_event.h"

namespace rex
{
    namespace events
    {
        class KeyTyped : public InputEvent
        {
        public:
            EVENT_CLASS_TYPE(KeyTyped);
            EVENT_CLASS_CATEGORY(EventCategory::INPUT | EventCategory::KEYBOARD);

            KeyTyped(const int8* text, const uint64 windowID, const uint32 timestamp, const memory::Blob& nativeEvent);
            ~KeyTyped() override;

            bool can_consume() const override;

            const int8* get_text() const;

            std::string to_string() const override;

        private:
            const int8* m_text;
        };
    }
}