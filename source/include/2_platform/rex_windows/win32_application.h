#pragma once

#include "core_application.h"

#include "event.h"

namespace rex
{
    struct FrameInfo;

    class DisplayManager;
    class LayerStack;
    class Layer;

    namespace events
    {
        class EventQueue;

        class WindowClose;
        class WindowResize;
    }

    namespace win32
    {
        class Window;
        class ApplicationLoop;

        namespace opengl
        {
            class Context;
        }

        class Application : public CoreApplication
        {
        public:
            Application(const ApplicationDescription& description);
            ~Application() override;

            bool is_marked_for_destruction() const;

            void quit() override;

        protected:
            void push_back_layer(std::unique_ptr<Layer> layer);

            bool platform_initialize() override;
            bool platform_shutdown() override;

            virtual void on_app_initialize();
            virtual void on_app_update(const FrameInfo& info);
            virtual void on_app_shutdown();

        private:
            void platform_update(const FrameInfo& info);
            void platform_event(events::Event& event);

            void process_events();
            void process_render_queue(const FrameInfo& info);
            void process_window();

            void mark_for_destroy();

            void create_display_manager();
            void create_event_queue();
            void create_layer_stack();
            void create_window();
            void create_input();
            void create_application_loop();
            void create_context();

            bool on_window_close(const events::WindowClose& evt);
            bool on_window_resize(const events::WindowResize& evt);

            bool m_is_marked_for_destruction;

            std::unique_ptr<DisplayManager> m_display_manager;
            std::unique_ptr<events::EventQueue> m_event_queue;
            std::unique_ptr<LayerStack> m_layer_stack;
            std::unique_ptr<Window> m_window;
            std::unique_ptr<ApplicationLoop> m_application_loop;
            std::unique_ptr<opengl::Context> m_context;
        };
    }
}