#pragma once

#include "event.h"

struct SDL_Window;
struct SDL_WindowEvent;
struct SDL_KeyboardEvent;
struct SDL_TextInputEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseWheelEvent;
struct SDL_MouseMotionEvent;

namespace rex
{
    namespace win32
    {
        class Window;

        class EventProcessor
        {
        public:
            enum class FocusDevice
            {
                MOUSE,
                KEYBOARD
            };

            EventProcessor(Window* window, events::EventCallbackFn callback);
            ~EventProcessor();

            void enable_event_processing();
            void disable_event_processing();

            void process_window_shown(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void process_window_hide(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void process_window_resize(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void process_window_close(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void process_window_focus_gained(SDL_WindowEvent& evt, FocusDevice device, const memory::Blob& nativeEvt);
            void process_window_focus_lost(SDL_WindowEvent& evt, FocusDevice device, const memory::Blob& nativeEvt);
            void process_window_minimized(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void process_window_maximized(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void process_window_exposed(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);

            void process_key(SDL_KeyboardEvent& evt, const memory::Blob& nativeEvt);
            void process_text_input(SDL_TextInputEvent& evt, const memory::Blob& nativeEvt);

            void process_mouse_button(SDL_MouseButtonEvent& evt, const memory::Blob& nativeEvt);
            void process_mouse_scroll(SDL_MouseWheelEvent& evt, const memory::Blob& nativeEvt);
            void process_mouse_move(SDL_MouseMotionEvent& evt, const memory::Blob& nativeEvt);

        private:
            void focus_gained_keyboard(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void focus_lost_keyboard(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void focus_gained_mouse(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);
            void focus_lost_mouse(SDL_WindowEvent& evt, const memory::Blob& nativeEvt);

            bool m_enabled;
            events::EventCallbackFn m_callback;
            Window* m_window;
        };
    }
}