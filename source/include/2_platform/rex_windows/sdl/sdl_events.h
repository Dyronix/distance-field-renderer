#pragma once

#include <string>
#include <SDL_events.h>

namespace rex
{
    namespace sdl
    {
        char* sdl_event_to_c_string(char* dst, size_t n, const SDL_Event* event);

        std::string sdl_event_to_string(const SDL_Event& event);
    }
}