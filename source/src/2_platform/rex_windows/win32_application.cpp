#include "rex_windows_pch.h"

#include "win32_application.h"
#include "win32_application_loop.h"
#include "win32_window.h"
#include "win32_context.h"

#include "input/win32_input.h"

#include "layer.h"
#include "layer_stack.h"

#include "event_dispatcher.h"
#include "event_queue.h"
#include "event_bus.h"

#include "window/window_close.h"
#include "window/window_resize.h"

#include "display.h"
#include "display_manager.h"

#include "world.h"
#include "frameinfo/frameinfo.h"
#include "frameinfo/fps.h"

#include "resources/resource_factory.h"
#include "resources/resources_api.h"

#include "renderer/renderer.h"
#include "renderer/renderer_api.h"

#include "sdl/sdl_sub_system.h"

#include <SDL.h>
#include <SDL_syswm.h>

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        const int32 g_main_display = 0;
        const int32 g_main_display_mode = 0;

        SDL_SysWMinfo g_sdl_system_info;

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
        const CoreWindow* Application::get_window() const
        {
            return m_window.get();
        }
        //-------------------------------------------------------------------------
        const CoreContext* Application::get_context() const
        {
            return m_context.get();
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
        void Application::push_back_layer(std::unique_ptr<Layer> layer)
        {
            m_layer_stack->push(std::move(layer));
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
                create_layer_stack();
                create_event_queue();
                create_window();
                create_input();
                create_context();
                create_application_loop();

                ResourceFactory::initialize(create_resources_api());
                Renderer::initialize(create_renderer_api());

                int32 window_width = m_window->get_width();
                int32 window_height = m_window->get_height();

                Renderer::set_viewport({0.0f, 0.0f}, {window_width, window_height});

                on_app_initialize();

                m_window->show();

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
            on_app_shutdown();

            m_layer_stack->clear();
            m_layer_stack.reset();

            events::EventBus::destroy_instance();

            Renderer::shutdown();
            ResourceFactory::shutdown();

            m_application_loop.reset();
            m_context.reset();

            Input::destroy_instance();

            m_window.reset();
            m_display_manager.reset();

            SDL_Quit();

            return true;
        }

        //-------------------------------------------------------------------------
        void Application::on_app_initialize()
        {
            // Implement in derived class
        }

        //-------------------------------------------------------------------------
        void Application::on_app_update(const FrameInfo& /*info*/)
        {
            // Implement in derived class
        }

        //-------------------------------------------------------------------------
        void Application::on_app_shutdown()
        {
            // Implement in derived class
        }

        //-------------------------------------------------------------------------
        void Application::platform_update(const FrameInfo& info)
        {
            process_events();
            process_render_queue(info);
            process_window(info);
        }
        //-------------------------------------------------------------------------
        void Application::platform_event(events::Event& event)
        {
            std::for_each(m_layer_stack->rbegin(), m_layer_stack->rend(), [&event](const std::unique_ptr<Layer>& layer) mutable
                          {
                              layer->handle_event(event);
                          });

            events::EventDispatcher dispatcher(event);
            dispatcher.dispatch<events::WindowClose>([&](const events::WindowClose& closeEvent)
                                                     {
                                                         return on_window_close(closeEvent);
                                                     });
            dispatcher.dispatch<events::WindowResize>([&](const events::WindowResize& resizeEvent)
                                                      {
                                                          return on_window_resize(resizeEvent);
                                                      });
        }

        //-------------------------------------------------------------------------
        void Application::process_events()
        {
            if (!m_event_queue->empty())
            {
                int32 pump_count = 0;
                events::Event evt = m_event_queue->next();
                while (evt != nullptr && pump_count < events::EventQueue::EVENT_QUEUE_PUMP_COUNT)
                {
                    platform_event(evt);

                    evt = m_event_queue->next();
                    ++pump_count;
                }
            }
        }
        //-------------------------------------------------------------------------
        void Application::process_render_queue(const FrameInfo& info)
        {
            bool is_visible = m_window->is_visible();

            if (is_visible)
            {
                Renderer::begin_frame();

                for (const std::unique_ptr<Layer>& layer : *m_layer_stack)
                {
                    if (layer->is_enabled())
                    {
                        layer->update(info);
                    }
                }

                on_app_update(info);

                Renderer::end_frame();
                Renderer::wait_and_render();

                m_context->present();
            }
        }
        //-------------------------------------------------------------------------
        void Application::process_window(const FrameInfo& info)
        {
            update_window_title(info.fps);

            m_window->update();
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
        void Application::create_layer_stack()
        {
            m_layer_stack = std::make_unique<LayerStack>();
        }

        //-------------------------------------------------------------------------
        void Application::create_event_queue()
        {
            m_event_queue = std::make_unique<events::EventQueue>();

            events::EventBus::create_instance();
            events::EventBus::instance()->push_event_queue(m_event_queue.get(), events::EventBusImpl::Activate::YES);
        }

        //-------------------------------------------------------------------------
        void Application::create_window()
        {
            uint32 window_flags = 0;
            window_flags |= (int32)WindowFlags::Flags::RESIZABLE;
            window_flags |= (int32)WindowFlags::Flags::ALLOW_HIGHDPI;
            window_flags |= (int32)WindowFlags::Flags::HIDDEN;

            WindowDescription window_description;

            window_description.title = get_application_description().name;
            window_description.width = get_application_description().window_width;
            window_description.height = get_application_description().window_height;
            window_description.fullscreen = get_application_description().fullscreen;
            window_description.display = m_display_manager->get_active();
            window_description.flags = WindowFlags((WindowFlags::Flags)window_flags);
            window_description.event_callback = [& event_queue = *m_event_queue](events::Event & event)
            {
                event_queue.enqueue(event);
            };

            m_window = std::make_unique<Window>(window_description);

            SDL_VERSION(&g_sdl_system_info.version);

            R_INFO("Window ({0}, {1}) intialized.", m_window->get_width(), m_window->get_height());
            R_INFO("\tusing subsystem: {0}", sdl::convert_to_sdl_subsystem_string(m_window->get_sdl_window(), &g_sdl_system_info).to_string());
        }
        //-------------------------------------------------------------------------
        void Application::create_input()
        {
            Input::create_instance(*m_window);
        }

        //-------------------------------------------------------------------------
        void Application::create_application_loop()
        {
            auto display = m_display_manager->get_active();
            auto display_mode = display->get_active_mode();

            auto fn = [&](const FrameInfo& info)
            {
                platform_update(info);
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

        //-------------------------------------------------------------------------
        bool Application::on_window_close(const events::WindowClose& evt)
        {
            uint32 main_window_id = SDL_GetWindowID(m_window->get_sdl_window());
            if (main_window_id != evt.get_window_id())
            {
                return false;
            }

            mark_for_destroy();

            return true;
        }
        //-------------------------------------------------------------------------
        bool Application::on_window_resize(const events::WindowResize& evt)
        {
            uint32 main_window_id = SDL_GetWindowID(m_window->get_sdl_window());
            if (main_window_id != evt.get_window_id())
            {
                return false;
            }

            if (!m_window->is_visible() || evt.get_width() == 0 || evt.get_height() == 0)
            {
                m_window->hide();
                return false;
            }

            rex::vec2 origin = rex::vec2(0.0f);
            rex::vec2 size = rex::vec2((float)evt.get_width(), (float)evt.get_height());

            Renderer::set_viewport(origin, size);

            return true;
        }

        //-------------------------------------------------------------------------
        void Application::update_window_title(const FPS& fps)
        {
            std::stringstream stream;

            stream << get_application_description().name;
            stream << " - FPS: ";
            stream << fps.get();

            m_window->set_title(create_sid(stream.str()));
        }

    } // namespace win32
}