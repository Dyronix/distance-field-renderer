#pragma once 

#include "core_window.h"

struct SDL_Window;

namespace rex
{
    class Display;

    namespace win32
    {
        class Window : public CoreWindow
        {
        public:
            Window(const WindowDescription& description);
            ~Window() override;

            void update() override;

            void show() override;
            void hide() override;

            bool set_windowed() override;
            bool set_fullscreen(const Display* display = nullptr) override;

            int32 get_width() const override;
            int32 get_height() const override;

            SDL_Window* get_sdl_window() const;

        private:
            void set_display_mode(const gsl::not_null<const Display*> display, int32 displayModeIndex = 0);

            SDL_Window* m_sdl_window;

            int32 m_windowed_width;
            int32 m_windowed_height;
        };
    }
}