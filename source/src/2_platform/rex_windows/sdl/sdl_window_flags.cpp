#include "rex_windows_pch.h"

#include "sdl/sdl_window_flags.h"

#include "core_window_flags.h"

#include <SDL_video.h>

namespace rex
{
    namespace sdl
    {
        //-------------------------------------------------------------------------
        uint32 convert_to_sdl_window_flags(const WindowFlags& flags)
        {
            uint32 sdl_flags = 0;

            if ((int32)flags.get() & (int32)WindowFlags::Flags::FULLSCREEN)
                sdl_flags |= SDL_WINDOW_FULLSCREEN;
            if ((int32)flags.get() & (int32)WindowFlags::Flags::FULLSCREEN_DESKTOP)
                sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

            if ((int32)flags.get() & (int32)WindowFlags::Flags::HIDDEN)
                sdl_flags |= SDL_WINDOW_HIDDEN;
            if ((int32)flags.get() & (int32)WindowFlags::Flags::BORDERLESS)
                sdl_flags |= SDL_WINDOW_BORDERLESS;
            if ((int32)flags.get() & (int32)WindowFlags::Flags::RESIZABLE)
                sdl_flags |= SDL_WINDOW_RESIZABLE;
            if ((int32)flags.get() & (int32)WindowFlags::Flags::MINIMIZED)
                sdl_flags |= SDL_WINDOW_MINIMIZED;
            if ((int32)flags.get() & (int32)WindowFlags::Flags::MAXIMIZED)
                sdl_flags |= SDL_WINDOW_MAXIMIZED;

            if ((int32)flags.get() & (int32)WindowFlags::Flags::INPUT_GRABBED)
                sdl_flags |= SDL_WINDOW_INPUT_GRABBED;
            if ((int32)flags.get() & (int32)WindowFlags::Flags::ALLOW_HIGHDPI)
                sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;

            return sdl_flags;
        }
    }
}