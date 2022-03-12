#pragma once

#include "input/key_event.h"

namespace rex
{
    namespace events
    {
        class KeyPressed : public KeyEvent
        {
        public:
            EVENT_CLASS_TYPE(KeyPressed);

            KeyPressed(const int32 key, const int16 modifiers, const int8 repeatCount, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);
            ~KeyPressed() override;

            const int16 get_modifiers() const;
            const int16 get_repeat_count() const;

            std::string to_string() const override;

        private:
            const int16 m_modifiers;
            const int8 m_repeat_count;
        };
    }
}