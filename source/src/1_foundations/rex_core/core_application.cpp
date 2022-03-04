#include "rex_core_pch.h"

#include "core_application.h"

#include "window.h"

#include "display.h"
#include "display_manager.h"

#include "world.h"

#include "frameinfo/frameinfo.h"

#include <SDL.h>

namespace rex
{
#ifdef __EMSCRIPTEN__
    //-------------------------------------------------------------------------
    void dispatch_main(ApplicationLoop* applicationLoop)
    {
        applicationLoop->loop();
    }
#endif

    //-------------------------------------------------------------------------
    const int32 g_main_display = 0;
    const int32 g_main_display_mode = 0;

    World g_world;

    //-------------------------------------------------------------------------
    CoreApplication* CoreApplication::s_instance = nullptr;

    //-------------------------------------------------------------------------
    CoreApplication::CoreApplication(const ApplicationDescription& description)
        :m_is_running(false)
        ,m_is_marked_for_destruction(false)
        ,m_description(description)
        ,m_display_manager(nullptr)
        ,m_window(nullptr)
    {
        R_INFO("Creating application: {0}", description.name);

        R_ASSERT_X(CoreApplication::s_instance == nullptr, "There can only be one application at the time");
        CoreApplication::s_instance = this;
    }

    //-------------------------------------------------------------------------
    CoreApplication::~CoreApplication()
    {
        m_is_running = false;

        R_INFO("Destroying application");
    }

    //-------------------------------------------------------------------------
    bool CoreApplication::is_running() const
    {
        return m_is_running;
    }

    //-------------------------------------------------------------------------
    bool CoreApplication::is_marked_for_destruction() const
    {
        return m_is_marked_for_destruction;
    }

    //-------------------------------------------------------------------------
    int32 CoreApplication::run()
    {
        initialize();
        update();
        shutdown();

        return EXIT_SUCCESS;
    }

    //-------------------------------------------------------------------------
    void CoreApplication::quit()
    {
        mark_for_destroy();
    }

    //-------------------------------------------------------------------------
    void CoreApplication::mark_for_destroy()
    {
        m_is_marked_for_destruction = true;
    }

    //-------------------------------------------------------------------------
    void CoreApplication::initialize()
    {
        /*
            Had some trouble with SDL_PumpEvents. 
            Sometimes (not always) it needs a very long time to return ( > 500000 Performance counts ).

            This is a bug in SDL 2.0.9
            The workaround is to add SDL_INIT_JOYSTICK ...

            https://discourse.libsdl.org/t/sdl-pumpevents-uses-long-time-sometimes-solved/25749
        */

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) == 0)
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
        }
        else
        {
            R_ERROR("Failed to initialize SDL!");
            return;
        }

        on_initialize();
    }

    //-------------------------------------------------------------------------
    void CoreApplication::update()
    {
#ifdef __EMSCRIPTEN__
        is_running = true;

        const int simulate_60_fps = 0;
        const int simulate_inf_loop = 1;

        emscripten_set_main_loop_arg((em_arg_callback_func)dispatch_main, this, simulate_60_fps, simulate_inf_loop);
#else
        m_is_running = true;

        while (m_is_running)
        {
            loop();

            m_is_running = !m_is_marked_for_destruction;
        }
#endif
    }

    //-------------------------------------------------------------------------
    void CoreApplication::shutdown()
    {
        on_shutdown();
    }

    //-------------------------------------------------------------------------
    void CoreApplication::loop()
    {
        FrameInfo info = { World::getDeltaTime(), World::getFramesPerSecond() };

        on_update(info);

        g_world.update();

        //
        // Cap framerate to "max_fps".
        // Safe resources of the machine we are running on.
        //
        auto active_display = m_display_manager->get_active();
        auto active_display_mode = active_display->get_active_mode();

        std::chrono::milliseconds actual_time(static_cast<uint32>(std::lrint(1000.0f / g_world.getFramesPerSecond().get())));
        std::chrono::milliseconds desired_time(static_cast<uint32>(std::lrint(1000.0f / active_display_mode->get_refresh_rate())));

        std::chrono::duration<float> elapsed_time = desired_time - actual_time;
        if (elapsed_time > std::chrono::milliseconds(0ms))
        {
            std::this_thread::sleep_for(elapsed_time);
        }
    }

    //-------------------------------------------------------------------------
    void CoreApplication::create_display_manager()
    {
        m_display_manager = std::make_unique<DisplayManager>();
        m_display_manager->set_active(g_main_display, g_main_display_mode);

        R_INFO("Display manager initialized with {0} display(s).", m_display_manager->display_count());
    }

    //-------------------------------------------------------------------------
    void CoreApplication::create_window()
    {
        WindowDescription window_description;

        window_description.title = m_description.name;
        window_description.width = m_description.window_width;
        window_description.height = m_description.window_height;
        window_description.fullscreen = m_description.fullscreen;
        window_description.display = m_display_manager->get_active();

        m_window = std::make_unique<Window>(window_description);

        R_INFO("Window ({0}, {1}) intialized.", m_window->get_width(), m_window->get_height());
    }

}