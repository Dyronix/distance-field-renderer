#include "rex_core_pch.h"

#include "display_mode.h"
#include "display.h"
#include "window.h"

#include <SDL.h>

namespace rex
{
    const int32 g_min_window_width = gsl::narrow<int32>(std::ceil(std::numeric_limits<int8>::max() * 0.5f));
    const int32 g_min_window_height = gsl::narrow<int32>(std::ceil(std::numeric_limits<int8>::max() * 0.5f));

    const int32 g_max_window_width = gsl::narrow<int32>(std::ceil(std::numeric_limits<int16>::max() * 0.5f));
    const int32 g_max_window_height = gsl::narrow<int32>(std::ceil(std::numeric_limits<int16>::max() * 0.5f));

    //-------------------------------------------------------------------------
    Window::Window(const WindowDescription& description)
        :m_sdl_window(nullptr)
        ,m_windowed_width(description.width)
        ,m_windowed_height(description.height)
    {
        R_ASSERT(m_windowed_width > g_min_window_width && m_windowed_width < g_max_window_width);
        R_ASSERT(m_windowed_height > g_min_window_height && m_windowed_height < g_max_window_height);

        int32 flags = 0;

        // flags |= SDL_WINDOW_OPENGL; we will create the OpenGL context ourselves
        flags |= SDL_WINDOW_ALLOW_HIGHDPI;
        flags |= SDL_WINDOW_INPUT_FOCUS;
        
        if (description.fullscreen)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        m_sdl_window = SDL_CreateWindow(description.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowed_width, m_windowed_height, flags);

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
        
    }

    //-------------------------------------------------------------------------
    void Window::show()
    {
        SDL_ShowWindow(m_sdl_window);
    }

    //-------------------------------------------------------------------------
    void Window::hide()
    {
        SDL_HideWindow(m_sdl_window);
    }

    //-------------------------------------------------------------------------
    bool Window::set_windowed()
    {
        if (SDL_SetWindowFullscreen(m_sdl_window, 0))
        {
            R_ERROR("Window could not be made windowed");
            return false;
        }

        SDL_SetWindowPosition(m_sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_SetWindowSize(m_sdl_window, m_windowed_width, m_windowed_height);

        return true;
    }

    //-------------------------------------------------------------------------
    bool Window::set_fullscreen(const Display* display)
    {
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

}