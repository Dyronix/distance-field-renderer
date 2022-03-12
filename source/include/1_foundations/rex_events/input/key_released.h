#pragma once

#include "input/key_event.h"

namespace rex
{
    namespace events
    {
        class KeyReleased : public KeyEvent
        {
        public:
            EVENT_CLASS_TYPE(KeyReleased);

            KeyReleased(const int32 key, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);
            ~KeyReleased() override;

            std::string to_string() const override;
        };
    }
}