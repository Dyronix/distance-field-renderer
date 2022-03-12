#pragma once

#include "event.h"

namespace rex
{
    template <typename T>
    struct Position
    {
        T x;
        T y;
    };

    template <typename T>
    struct Info
    {
        Position<T> screen_position;
        Position<T> local_position;
    };

    namespace events
    {
        class InputEvent : public BaseEvent
        {
        public:
            ~InputEvent() override;

            const uint64 get_window_id() const;
            const uint32 get_time_stamp() const;

        protected:
            InputEvent(const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent);

        private:
            const uint64 m_window_id;
            const uint32 m_time_stamp;
        };
    }
}