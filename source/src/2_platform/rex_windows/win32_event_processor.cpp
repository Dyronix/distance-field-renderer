#include "rex_windows_pch.h"

#include "win32_event_processor.h"

// input events
#include "input/key_pressed.h"
#include "input/key_released.h"
#include "input/key_typed.h"
#include "input/mouse_move.h"
#include "input/mouse_button_released.h"
#include "input/mouse_button_pressed.h"
#include "input/mouse_scroll.h"

// window events
#include "window/window_lost_keyboard_focus.h"
#include "window/window_lost_mouse_focus.h"
#include "window/window_gained_keyboard_focus.h"
#include "window/window_gained_mouse_focus.h"
#include "window/window_close.h"
#include "window/window_resize.h"
#include "window/window_minized.h"
#include "window/window_maximized.h"
#include "window/window_exposed.h"
#include "window/window_show.h"
#include "window/window_hide.h"

// utilities
#include "win32_window.h"

// sdl
#include <SDL_events.h>

namespace rex
{
    namespace win32
    {
        using MouseCode = uint16;

        enum class Action
        {
            NONE,
            PRESSED,
            RELEASED
        };

        //-------------------------------------------------------------------------
        template <typename TEvent>
        Action to_action(TEvent& evt)
        {
            switch (evt.state)
            {
                case SDL_PRESSED: return Action::PRESSED;
                case SDL_RELEASED: return Action::RELEASED;
                default: R_ASSERT_X(false, "Unsupported Action"); return Action::NONE;
            }
        }

        //-------------------------------------------------------------------------
        MouseKeyMetaData make_mouse_key_meta_data()
        {
            const uint8* keystate = SDL_GetKeyboardState(nullptr);
            if (keystate == nullptr || std::strcmp((const char*)keystate, "") == 0)
            {
                return {};
            }

            MouseKeyMetaData meta_data;
            meta_data.is_ctrl_down = keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL];
            meta_data.is_shift_down = keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT];

            return meta_data;
        }

        //-------------------------------------------------------------------------
        ScrollData make_scroll_data(SDL_MouseWheelEvent& evt)
        {
            ScrollMetaData meta_data_x;
            meta_data_x.direction = evt.x >= 0 ? ScrollDirection::RIGHT : ScrollDirection::LEFT;
            meta_data_x.wheel_delta = gsl::narrow<float>(evt.x);

            ScrollMetaData meta_data_y;
            meta_data_y.direction = evt.y >= 0 ? ScrollDirection::AWAY_FROM_USER : ScrollDirection::TOWARDS_USER;
            meta_data_y.wheel_delta = gsl::narrow<float>(evt.y);

            return {{ScrollAxis::X_AXIS, meta_data_x}, {ScrollAxis::Y_AXIS, meta_data_y}};
        }
        //-------------------------------------------------------------------------
        MouseDelta make_mouse_delta(SDL_MouseMotionEvent& evt)
        {
            return {evt.xrel, evt.yrel};
        }
        //-------------------------------------------------------------------------
        template <typename TEvent>
        MouseInfo make_mouse_info(SDL_Window* window, const TEvent& evt)
        {
            int32 x;
            int32 y;
            SDL_GetWindowPosition(window, &x, &y);

            MouseInfo mouse_info;
            mouse_info.local_position = {evt.x, evt.y};
            mouse_info.screen_position = {x + evt.x, y + evt.y};

            return mouse_info;
        }

        //-------------------------------------------------------------------------
        EventProcessor::EventProcessor(Window* window, events::EventCallbackFn callback)
            : m_window(window)
            , m_callback(callback)
            , m_enabled(false)
        {
        }
        //-------------------------------------------------------------------------
        EventProcessor::~EventProcessor() = default;

        //-------------------------------------------------------------------------
        void EventProcessor::enable_event_processing()
        {
            m_enabled = true;
        }
        //-------------------------------------------------------------------------
        void EventProcessor::disable_event_processing()
        {
            m_enabled = false;
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_window_shown(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowShow>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }
        //-------------------------------------------------------------------------
        void EventProcessor::process_window_hide(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowHide>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }
        //-------------------------------------------------------------------------
        void EventProcessor::process_window_resize(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            uint16 new_width = gsl::narrow<uint16>(evt.data1);
            uint16 new_height = gsl::narrow<uint16>(evt.data2);

            auto rex_evt = events::make_new<events::WindowResize>(new_width, new_height, evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }
        //-------------------------------------------------------------------------
        void EventProcessor::process_window_close(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(evt);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowClose>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_window_focus_gained(SDL_WindowEvent& evt, FocusDevice device, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            switch (device)
            {
                case FocusDevice::KEYBOARD: focus_gained_keyboard(evt, nativeEvt); break;
                case FocusDevice::MOUSE: focus_gained_mouse(evt, nativeEvt); break;
            }
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_window_focus_lost(SDL_WindowEvent& evt, FocusDevice device, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            switch (device)
            {
                case FocusDevice::KEYBOARD: focus_lost_keyboard(evt, nativeEvt); break;
                case FocusDevice::MOUSE: focus_lost_mouse(evt, nativeEvt); break;
            }
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_window_minimized(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(evt);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowMinimized>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_window_maximized(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(evt);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowMaximized>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_window_exposed(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(evt);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowExposed>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_key(SDL_KeyboardEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            uint16 keycode = gsl::narrow<uint16>(evt.keysym.scancode);

            switch (to_action(evt))
            {
                case Action::PRESSED:
                {
                    uint16 mods = evt.keysym.mod;

                    uint8 repeat = evt.repeat;

                    auto rex_evt = events::make_new<events::KeyPressed>(keycode, mods, repeat, evt.windowID, evt.timestamp, nativeEvt);
                    m_callback(rex_evt);
                }
                break;
                case Action::RELEASED:
                {
                    auto rex_evt = events::make_new<events::KeyReleased>(keycode, evt.windowID, evt.timestamp, nativeEvt);
                    m_callback(rex_evt);
                }
                break;
                default: R_ASSERT_X(false, "Unsupported Action"); return;
            }
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_text_input(SDL_TextInputEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            const int8* text = (const int8*)&evt.text[0];

            auto rex_evt = events::make_new<events::KeyTyped>(text, evt.windowID, evt.timestamp, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::process_mouse_button(SDL_MouseButtonEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            MouseCode mouse_code = evt.button;
            MouseInfo mouse_info = make_mouse_info(m_window->get_sdl_window(), evt);

            switch (to_action(evt))
            {
                case Action::PRESSED:
                {
                    MouseKeyMetaData meta_data = make_mouse_key_meta_data();

                    auto rex_evt = events::make_new<events::MouseButtonPressed>(meta_data, mouse_code, mouse_info, evt.windowID, evt.timestamp, nativeEvt);
                    m_callback(rex_evt);
                }
                break;
                case Action::RELEASED:
                {
                    auto rex_evt = events::make_new<events::MouseButtonReleased>(mouse_code, mouse_info, evt.windowID, evt.timestamp, nativeEvt);
                    m_callback(rex_evt);
                }
                break;
                default: R_ASSERT_X(false, "Unsupported Action"); return;
            }
        }
        //-------------------------------------------------------------------------
        void EventProcessor::process_mouse_scroll(SDL_MouseWheelEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            MouseKeyMetaData meta_data = make_mouse_key_meta_data();
            ScrollData scroll_data = make_scroll_data(evt);
            MouseInfo mouse_info = make_mouse_info(m_window->get_sdl_window(), evt);

            auto rex_evt = events::make_new<events::MouseScroll>(meta_data, scroll_data, mouse_info, evt.windowID, evt.timestamp, nativeEvt);
            m_callback(rex_evt);
        }
        //-------------------------------------------------------------------------
        void EventProcessor::process_mouse_move(SDL_MouseMotionEvent& evt, const memory::Blob& nativeEvt)
        {
            if (!m_enabled)
                return;

            MouseKeyMetaData meta_data = make_mouse_key_meta_data();
            MouseInfo mouse_info = make_mouse_info(m_window->get_sdl_window(), evt);
            MouseDelta mouse_delta = make_mouse_delta(evt);

            auto rex_evt = events::make_new<events::MouseMove>(meta_data, mouse_info, mouse_delta, evt.windowID, evt.timestamp, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::focus_gained_keyboard(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(m_window);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowGainedKeyboardFocus>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }
        //-------------------------------------------------------------------------
        void EventProcessor::focus_lost_keyboard(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(m_window);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowLostKeyboardFocus>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }

        //-------------------------------------------------------------------------
        void EventProcessor::focus_gained_mouse(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(m_window);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowGainedMouseFocus>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }
        //-------------------------------------------------------------------------
        void EventProcessor::focus_lost_mouse(SDL_WindowEvent& evt, const memory::Blob& nativeEvt)
        {
            UNUSED_PARAM(m_window);

            if (!m_enabled)
                return;

            auto rex_evt = events::make_new<events::WindowLostMouseFocus>(evt.windowID, nativeEvt);
            m_callback(rex_evt);
        }
    }
}