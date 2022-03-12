#pragma once

#include "core_window.h"
#include "blob.h"

struct SDL_Window;
struct SDL_WindowEvent;

namespace rex
{
    class Display;

    namespace win32
    {
        class EventProcessor;
        class EventHandler;

        class Window : public CoreWindow
        {
        public:
            Window(const WindowDescription& description);
            ~Window() override;

            void update() override;

            void show() override;
            void hide() override;

            void gained_focus(const Focus::Type& focus) override;
            void lost_focus(const Focus::Type& focus) override;

            bool is_visible() const override;
            bool is_windowed() const override;
            bool is_fullscreen() const override;

            bool set_title(const StringID& title) override;
            bool set_windowed() override;
            bool set_fullscreen(const Display* display = nullptr) override;

            int32 get_width() const override;
            int32 get_height() const override;

            SDL_Window* get_sdl_window() const;

        private:
            void set_display_mode(const gsl::not_null<const Display*> display, int32 displayModeIndex = 0);
            void handle_window_events(const SDL_WindowEvent& windowEvent);

            SDL_Window* m_sdl_window;

            int32 m_windowed_width;
            int32 m_windowed_height;

            int32 m_window_presentation;

            Focus m_keyboard_focus;
            Focus m_mouse_focus;

            std::unique_ptr<EventProcessor> m_event_processor;
            std::unique_ptr<EventHandler> m_event_handler;
        };
    }
}