#pragma once

namespace rex
{
    class WindowFlags;

    namespace sdl
    {
        uint32 convert_to_sdl_window_flags(const WindowFlags& flags);
    }
}