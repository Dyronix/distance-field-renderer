#pragma once

#include "memory/internal_ptr.h"

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
        class EventProcessor;
        
        struct WindowCallbackFunctions
        {
            using WindowCallbackFn = std::function<void(SDL_WindowEvent)>;
            using KeyboardCallbackFn = std::function<void(SDL_KeyboardEvent)>;
            using TextInputCallbackFn = std::function<void(SDL_TextInputEvent)>;
            using MouseButtonCallbackFn = std::function<void(SDL_MouseButtonEvent)>;
            using MouseWheelCallbackFn = std::function<void(SDL_MouseWheelEvent)>;
            using MouseMotionCallbackFn = std::function<void(SDL_MouseMotionEvent)>;

            WindowCallbackFn window_callback;
            KeyboardCallbackFn keyboard_callback;
            TextInputCallbackFn text_input_callback;
            MouseButtonCallbackFn mouse_button_callback;
            MouseWheelCallbackFn mouse_wheel_callback;
            MouseMotionCallbackFn mouse_motion_callback;
        };

        class EventHandler
        {
        public:
            EventHandler(EventProcessor& eventProcessor, const WindowCallbackFunctions& callbackFunctions);
            ~EventHandler();

            void handle_events();

        private:
            struct Internal;
            rex::internal_scope_ptr<Internal> m_internal;
        };
    }
}