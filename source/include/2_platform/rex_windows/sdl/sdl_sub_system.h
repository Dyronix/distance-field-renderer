#pragma once

struct SDL_Window;
struct SDL_SysWMinfo;

namespace rex
{
    namespace sdl
    {
        StringID convert_to_sdl_subsystem_string(SDL_Window* window, SDL_SysWMinfo* info);
    }
}