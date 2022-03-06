#pragma once

#include "core_application.h"

namespace rex
{
    class DisplayManager;

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
            bool platform_initialize() override;
            bool platform_shutdown() override;

        private:
            void mark_for_destroy();

            void create_display_manager();
            void create_window();
            void create_application_loop();
            void create_context();

            bool m_is_marked_for_destruction;

            std::unique_ptr<DisplayManager> m_display_manager;
            std::unique_ptr<Window> m_window;
            std::unique_ptr<ApplicationLoop> m_application_loop;
            std::unique_ptr<opengl::Context> m_context;
        };
    }
}