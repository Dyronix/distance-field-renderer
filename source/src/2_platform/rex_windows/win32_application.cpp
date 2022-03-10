#include "rex_windows_pch.h"

#include "win32_application.h"
#include "win32_application_loop.h"
#include "win32_window.h"
#include "win32_context.h"

#include "display.h"
#include "display_manager.h"

#include "world.h"

#include <SDL.h>

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        const int32 g_main_display = 0;
        const int32 g_main_display_mode = 0;

        //-------------------------------------------------------------------------
        Application::Application(const ApplicationDescription& description)
            : CoreApplication(description)
            , m_is_marked_for_destruction(false)
        {
        }

        //-------------------------------------------------------------------------
        Application::~Application()
        {
        }

        //-------------------------------------------------------------------------
        bool Application::is_marked_for_destruction() const
        {
            return m_is_marked_for_destruction;
        }

        //-------------------------------------------------------------------------
        void Application::quit()
        {
            if (is_marked_for_destruction())
            {
                R_ERROR("Application already prepared for destruction.");
                return;
            }

            mark_for_destroy();
        }

        //-------------------------------------------------------------------------
        bool Application::platform_initialize()
        {
            /*
                Had some trouble with SDL_PumpEvents.
                Sometimes (not always) it needs a very long time to return ( > 500000 Performance counts ).

                This is a bug in SDL 2.0.9
                The workaround is to add SDL_INIT_JOYSTICK ...

                https://discourse.libsdl.org/t/sdl-pumpevents-uses-long-time-sometimes-solved/25749
            */

            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 0)
            {
                R_INFO("Successfully initialized SDL!");

#if REX_API_OPENGL
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif REX_API_OPENGLES
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

                create_display_manager();
                create_window();
                create_application_loop();
                create_context();

                // Run the main application loop!
                m_application_loop->exec();

                return true;
            }

            R_ERROR("Failed to initialize SDL!");
            R_ERROR(SDL_GetError());

            return false;
        }

        //-------------------------------------------------------------------------
        bool Application::platform_shutdown()
        {
            m_context.reset();
            m_application_loop.reset();
            m_window.reset();
            m_display_manager.reset();

            SDL_Quit();

            return true;
        }

        //-------------------------------------------------------------------------
        void Application::mark_for_destroy()
        {
            m_is_marked_for_destruction = true;

            m_application_loop->stop();

            R_INFO("Mark application for destruction.");
        }

        //-------------------------------------------------------------------------
        void Application::create_display_manager()
        {
            m_display_manager = std::make_unique<DisplayManager>();
            m_display_manager->set_active(g_main_display, g_main_display_mode);

            R_INFO("Display manager initialized with {0} display(s).", m_display_manager->display_count());
        }

        //-------------------------------------------------------------------------
        void Application::create_window()
        {
            WindowDescription window_description;

            window_description.title = get_application_description().name;
            window_description.width = get_application_description().window_width;
            window_description.height = get_application_description().window_height;
            window_description.fullscreen = get_application_description().fullscreen;
            window_description.display = m_display_manager->get_active();

            m_window = std::make_unique<Window>(window_description);

            R_INFO("Window ({0}, {1}) intialized.", m_window->get_width(), m_window->get_height());
        }

        //-------------------------------------------------------------------------
        void Application::create_application_loop()
        {
            auto display = m_display_manager->get_active();
            auto display_mode = display->get_active_mode();

            auto fn = [](const FrameInfo& /*info*/)
            {
                // client code update ...
            };

            m_application_loop = std::make_unique<ApplicationLoop>(fn, display_mode->get_refresh_rate());

            R_INFO("Application loop ({0} hz) initialized.", display_mode->get_refresh_rate());
        }

        //-------------------------------------------------------------------------
        void Application::create_context()
        {
            m_context = std::make_unique<opengl::Context>(m_window->get_sdl_window());
            m_context->make_current();

            R_INFO("OpenGL Context initialized.");
            R_INFO("\tversion: {0}", m_context->get_version());
            R_INFO("\trenderer: {0}", m_context->get_renderer());
            R_INFO("\tvendor: {0}", m_context->get_vendor());
        }
    }
}