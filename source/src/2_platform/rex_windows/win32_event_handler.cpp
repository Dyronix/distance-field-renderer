#include "rex_windows_pch.h"

#include "win32_event_handler.h"
#include "win32_event_processor.h"

#include "event_queue.h"

#include <SDL.h>
#include <SDL_events.h>

namespace rex
{
    namespace win32
    {
        struct EventHandler::Internal
        {
            Internal(EventProcessor& eventProcessor, const WindowCallbackFunctions& callbackFunctions)
                : event_processor(eventProcessor)
                , event_callbacks(callbackFunctions)
            {
            }

            //-------------------------------------------------------------------------
            template <typename T, typename U>
            void invoke_callback(const T& callback, const U& event)
            {
                if (callback)
                {
                    callback(event);
                }
            }

            //-------------------------------------------------------------------------
            void handle_window_events(SDL_Event& event, const memory::Blob& nativeEvent)
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_SHOWN:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_shown(event.window, nativeEvent);
                        break;
                    case SDL_WINDOWEVENT_HIDDEN:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_hide(event.window, nativeEvent);
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_close(event.window, nativeEvent);
                        break;

                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        invoke_callback(event_callbacks.window_callback, event.window);

                        event_processor.process_window_resize(event.window, nativeEvent);
                    }
                    break;

                    case SDL_WINDOWEVENT_MINIMIZED:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_minimized(event.window, nativeEvent);
                        break;

                    case SDL_WINDOWEVENT_MAXIMIZED:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_maximized(event.window, nativeEvent);
                        break;

                    case SDL_WINDOWEVENT_EXPOSED:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_exposed(event.window, nativeEvent);
                        break;

                    case SDL_WINDOWEVENT_ENTER:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_focus_gained(event.window, EventProcessor::FocusDevice::MOUSE, nativeEvent);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_focus_gained(event.window, EventProcessor::FocusDevice::KEYBOARD, nativeEvent);
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_focus_gained(event.window, EventProcessor::FocusDevice::MOUSE, nativeEvent);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        invoke_callback(event_callbacks.window_callback, event.window);
                        event_processor.process_window_focus_gained(event.window, EventProcessor::FocusDevice::KEYBOARD, nativeEvent);
                        break;
                }
            }
            //-------------------------------------------------------------------------
            void handle_events()
            {
                // Pumps the event loop, gathering events from the input devices.
                //
                SDL_PumpEvents();

                // Checks the event queue for messages and optionally returns them.
                //      returns the number of events actually stored, or -1 if there was an error.
                //
                int32 num_events = SDL_PeepEvents(sdl_event_buffer, events::EventQueue::EVENT_QUEUE_CHANNELS, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

                for (int32 i = 0; i < num_events; ++i)
                {
                    SDL_Event& event = sdl_event_buffer[i];

                    memory::Blob native_event_blob = make_blob((memory::byte*)&event, sizeof(SDL_Event));

                    switch (event.type)
                    {
                        // Window
                        case SDL_WINDOWEVENT:
                            handle_window_events(event, native_event_blob);
                            break;

                        // Text
                        case SDL_TEXTINPUT:
                            invoke_callback(event_callbacks.text_input_callback, event.text);
                            event_processor.process_text_input(event.text, native_event_blob);
                            break;

                        // Keyboard
                        case SDL_KEYUP:
                            invoke_callback(event_callbacks.keyboard_callback, event.key);
                            event_processor.process_key(event.key, native_event_blob);
                            break;
                        case SDL_KEYDOWN:
                            invoke_callback(event_callbacks.keyboard_callback, event.key);
                            event_processor.process_key(event.key, native_event_blob);
                            break;

                        // Mouse
                        case SDL_MOUSEMOTION:
                            invoke_callback(event_callbacks.mouse_motion_callback, event.motion);
                            event_processor.process_mouse_move(event.motion, native_event_blob);
                            break;
                        case SDL_MOUSEBUTTONUP:
                            invoke_callback(event_callbacks.mouse_button_callback, event.button);
                            event_processor.process_mouse_button(event.button, native_event_blob);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            invoke_callback(event_callbacks.mouse_button_callback, event.button);
                            event_processor.process_mouse_button(event.button, native_event_blob);
                            break;
                        case SDL_MOUSEWHEEL:
                            invoke_callback(event_callbacks.mouse_wheel_callback, event.wheel);
                            event_processor.process_mouse_scroll(event.wheel, native_event_blob);
                            break;
                    }
                }
            }

            EventProcessor& event_processor;
            WindowCallbackFunctions event_callbacks;

            SDL_Event sdl_event_buffer[events::EventQueue::EVENT_QUEUE_CHANNELS];
        };

        //-------------------------------------------------------------------------
        EventHandler::EventHandler(EventProcessor& eventProcessor, const WindowCallbackFunctions& callbackFunctions)
            : m_internal(rex::make_internal_scope_ptr<Internal>(eventProcessor, callbackFunctions))
        {
        }
        //-------------------------------------------------------------------------
        EventHandler::~EventHandler()
        {
        }

        //-------------------------------------------------------------------------
        void EventHandler::handle_events()
        {
            m_internal->handle_events();
        }
    }
}