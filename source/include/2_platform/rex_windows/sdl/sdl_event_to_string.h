#ifndef SDL_EVENT_TO_STRING_H
#define SDL_EVENT_TO_STRING_H

#include <string>
#include <SDL_events.h>

char *sdl_event_to_c_string(char *dst, size_t n, const SDL_Event *event);

std::string sdl_event_to_string(const SDL_Event& event);

void sdl_event_to_sdl_log(const SDL_Event& event);

#endif

