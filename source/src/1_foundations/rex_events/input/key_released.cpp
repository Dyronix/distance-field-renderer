#include "rex_events_pch.h"
#include "input/key_released.h"

namespace rex
{
    namespace events
    {
        //-------------------------------------------------------------------------
        KeyReleased::KeyReleased(const int32 key, const uint64 windowID, const uint32 timeStamp, const memory::Blob& nativeEvent)
            : KeyEvent(key, windowID, timeStamp, nativeEvent)
        {
            R_TRACE("Key released event created");
        }
        //-------------------------------------------------------------------------
        KeyReleased::~KeyReleased() = default;

        //-------------------------------------------------------------------------
        std::string KeyReleased::to_string() const
        {
            std::stringstream s;
            s << "Key Released: key: " << get_key();
            return s.str();
        }
    }
}