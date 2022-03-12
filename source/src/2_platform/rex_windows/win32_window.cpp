#include "rex_windows_pch.h"

#include "win32_window.h"
#include "win32_event_processor.h"
#include "win32_event_handler.h"

#include "sdl/sdl_window_flags.h"

#include "display_mode.h"
#include "display.h"

#include <SDL.h>
#include <SDL_events.h>

namespace rex
{
    namespace win32
    {
        const int32 g_min_window_width = gsl::narrow<int32>(std::ceil(std::numeric_limits<int8>::max() * 0.5f));
        const int32 g_min_window_height = gsl::narrow<int32>(std::ceil(std::numeric_limits<int8>::max() * 0.5f));

        const int32 g_max_window_width = gsl::narrow<int32>(std::ceil(std::numeric_limits<int16>::max() * 0.5f));
        const int32 g_max_window_height = gsl::narrow<int32>(std::ceil(std::numeric_limits<int16>::max() * 0.5f));

        namespace window_presentation
        {
            const int32 VISIBLE = BIT(0);
            const int32 WINDOWED = BIT(1);
            const int32 FULLSCREEN = BIT(2);
        }

        //-------------------------------------------------------------------------
        Window::Window(const WindowDescription& description)
            : m_sdl_window(nullptr)
            , m_windowed_width(description.width)
            , m_windowed_height(description.height)
            , m_window_presentation(0)
            , m_keyboard_focus({Focus::Type::KEYBOARD, false})
            , m_mouse_focus({Focus::Type::MOUSE, false})
            , m_event_processor(nullptr)
            , m_event_handler(nullptr)
        {
            R_ASSERT(m_windowed_width > g_min_window_width && m_windowed_width < g_max_window_width);
            R_ASSERT(m_windowed_height > g_min_window_height && m_windowed_height < g_max_window_height);

            m_event_processor = std::make_unique<EventProcessor>(this, description.event_callback);
            m_event_processor->enable_event_processing();

            WindowCallbackFunctions callback_functions;

            callback_functions.window_callback = [&](const SDL_WindowEvent& event)
            {
                handle_window_events(event);
            };

            m_event_handler = std::make_unique<EventHandler>(*m_event_processor, callback_functions);

            uint32 sdl_window_flags = sdl::convert_to_sdl_window_flags(description.flags);
            sdl_window_flags |= SDL_WINDOW_OPENGL;
            sdl_window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
            sdl_window_flags |= SDL_WINDOW_INPUT_FOCUS;

            if (description.fullscreen)
            {
                sdl_window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            }
            else
            {
                sdl_window_flags |= SDL_WINDOW_RESIZABLE;
            }

            m_sdl_window = SDL_CreateWindow(description.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowed_width, m_windowed_height, sdl_window_flags);

            if (m_sdl_window == nullptr)
            {
                R_ERROR("Window creation failed!");
                return;
            }

            if (description.fullscreen && description.display != nullptr)
            {
                set_display_mode(description.display, description.display_mode_index);
            }
        }

        //-------------------------------------------------------------------------
        Window::~Window()
        {
            SDL_DestroyWindow(m_sdl_window);
        }

        //-------------------------------------------------------------------------
        void Window::update()
        {
            m_event_handler->handle_events();
        }

        //-------------------------------------------------------------------------
        void Window::show()
        {
            if (is_visible())
            {
                return;
            }

            SDL_ShowWindow(m_sdl_window);

            m_window_presentation &= window_presentation::VISIBLE;
        }

        //-------------------------------------------------------------------------
        void Window::hide()
        {
            if (!is_visible())
            {
                return;
            }

            SDL_HideWindow(m_sdl_window);

            m_window_presentation &= ~window_presentation::VISIBLE;
        }

        //-------------------------------------------------------------------------
        void Window::gained_focus(const Focus::Type& focus)
        {
            switch (focus)
            {
                case Focus::Type::KEYBOARD: m_keyboard_focus.acquired = true; break;
                case Focus::Type::MOUSE: m_mouse_focus.acquired = true; break;

                default: R_ERROR("Unknown focus type: {0}", (int32)focus); break;
            }
        }
        //-------------------------------------------------------------------------
        void Window::lost_focus(const Focus::Type& focus)
        {
            switch (focus)
            {
                case Focus::Type::KEYBOARD: m_keyboard_focus.acquired = false; break;
                case Focus::Type::MOUSE: m_mouse_focus.acquired = false; break;

                default: R_ERROR("Unknown focus type: {0}", (int32)focus); break;
            }
        }

        //-------------------------------------------------------------------------
        bool Window::is_visible() const
        {
            return (m_window_presentation & window_presentation::VISIBLE);
        }
        //-------------------------------------------------------------------------
        bool Window::is_windowed() const
        {
            return (m_window_presentation & window_presentation::WINDOWED);
        }
        //-------------------------------------------------------------------------
        bool Window::is_fullscreen() const
        {
            return (m_window_presentation & window_presentation::FULLSCREEN);
        }

        //-------------------------------------------------------------------------
        bool Window::set_title(const StringID& title)
        {
            char* name;
            size_t size;

            title.to_string(&name, size);

            SDL_SetWindowTitle(m_sdl_window, name);

            const char* error = SDL_GetError();
            if (error == nullptr)
            {
                return true;
            }

            R_ERROR("SDL: {0}", error);
            return false;
        }

        //-------------------------------------------------------------------------
        bool Window::set_windowed()
        {
            if (is_windowed())
            {
                return true;
            }

            if (SDL_SetWindowFullscreen(m_sdl_window, 0))
            {
                R_ERROR("Window could not be made windowed");
                return false;
            }

            SDL_SetWindowPosition(m_sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            SDL_SetWindowSize(m_sdl_window, m_windowed_width, m_windowed_height);

            m_window_presentation &= window_presentation::WINDOWED;
            m_window_presentation &= ~window_presentation::FULLSCREEN;

            return true;
        }

        //-------------------------------------------------------------------------
        bool Window::set_fullscreen(const Display* display)
        {
            if (is_fullscreen())
            {
                return true;
            }

            m_windowed_width = get_width();
            m_windowed_height = get_height();

            if (SDL_SetWindowFullscreen(m_sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
            {
                R_ERROR("Window could not be made fullscreen");
                return false;
            }

            if (display != nullptr)
            {
                set_display_mode(display);
            }

            m_window_presentation &= ~window_presentation::WINDOWED;
            m_window_presentation &= window_presentation::FULLSCREEN;

            return true;
        }

        //-------------------------------------------------------------------------
        int32 Window::get_width() const
        {
            int32 width = 0;
            int32 height = 0;

            SDL_GetWindowSize(m_sdl_window, &width, &height);

            return width;
        }

        //-------------------------------------------------------------------------
        int32 Window::get_height() const
        {
            int32 width = 0;
            int32 height = 0;

            SDL_GetWindowSize(m_sdl_window, &width, &height);

            return height;
        }

        //-------------------------------------------------------------------------
        SDL_Window* Window::get_sdl_window() const
        {
            return m_sdl_window;
        }

        //-------------------------------------------------------------------------
        void Window::set_display_mode(const gsl::not_null<const Display*> display, int32 displayModeIndex)
        {
            auto display_mode = display->get_display_mode(displayModeIndex);
            if (!display_mode)
            {
                R_ERROR("Display mode with index: {0} for display with name: {1} was not found", displayModeIndex, display->get_name());
                return;
            }

            SDL_DisplayMode mode;

            if (SDL_GetDisplayMode(display->get_display_index(), display_mode->get_mode_index(), &mode))
            {
                R_ERROR("Could not retrieve display mode for display with name: {0}", display->get_name());
                return;
            }

            if (SDL_SetWindowDisplayMode(m_sdl_window, &mode))
            {
                R_WARN("Could not set given display mode, scanning for closest related display mode ...");

                SDL_DisplayMode closest;
                if (SDL_GetClosestDisplayMode(display->get_display_index(), &mode, &closest) == NULL)
                {
                    R_ERROR("Could not retrieve closest display mode for display with name: {0}", display->get_name());
                    return;
                }

                if (SDL_SetWindowDisplayMode(m_sdl_window, &closest))
                {
                    R_WARN("Could not set given closest display mode, reverting to desktop display mode ...");

                    if (SDL_GetDesktopDisplayMode(display->get_display_index(), &mode))
                    {
                        R_ERROR("Could not retrieve desktop display mode for display with name: {0}", display->get_name());
                        return;
                    }

                    if (SDL_SetWindowDisplayMode(m_sdl_window, &mode))
                    {
                        R_ERROR("Could not set desktop display mode");
                        return;
                    }
                }
            }

            R_INFO("DisplayMode mode set on display with name: {0}", display->get_name());
        }

        //-------------------------------------------------------------------------
        void Window::handle_window_events(const SDL_WindowEvent& event)
        {
            switch (event.event)
            {
                case SDL_WINDOWEVENT_ENTER: { gained_focus(Focus::Type::MOUSE);
                }
                break;
                case SDL_WINDOWEVENT_FOCUS_GAINED: { gained_focus(Focus::Type::KEYBOARD);
                }
                break;
                case SDL_WINDOWEVENT_LEAVE: { lost_focus(Focus::Type::MOUSE);
                }
                break;
                case SDL_WINDOWEVENT_FOCUS_LOST: { lost_focus(Focus::Type::KEYBOARD);
                }
                break;
                case SDL_WINDOWEVENT_SHOWN: { m_window_presentation &= window_presentation::VISIBLE;
                }
                break;
                case SDL_WINDOWEVENT_HIDDEN: { m_window_presentation &= ~window_presentation::VISIBLE;
                }
                break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    m_windowed_width = event.data1;
                    m_windowed_height = event.data2;
                }
                break;
            }
        }
    }
}