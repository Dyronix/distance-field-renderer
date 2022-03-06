#pragma once

#include "core_context.h"

struct SDL_Window;

namespace rex
{
    namespace win32
    {
        namespace opengl
        {
            class Context : public CoreContext
            {
            public:
                Context(SDL_Window* window, const SwapInterval& interval = SwapInterval::SYNCHRONIZED_VERTICAL_UPDATE);
                ~Context() override;

                void* get_sdl_context() const;

                const char* get_version() const override;
                const char* get_renderer() const override;
                const char* get_vendor() const override;

                void set_vertical_update(const SwapInterval& interval);

                void make_current();
                void present();

            private:
                SDL_Window* m_sdl_window;
                void* m_sdl_context;
            };
        }
    }
}