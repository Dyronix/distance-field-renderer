#pragma once

namespace rex
{
    class ApplicationArguments;
    class Window;
    class DisplayManager;

    struct FrameInfo;

    struct ApplicationDescription
    {
        const char* name;

        int32 window_width;
        int32 window_height;

        bool fullscreen;
    };

    struct ApplicationLoop
    {
        virtual ~ApplicationLoop() = default;

        virtual void loop() = 0;
    };

    class CoreApplication : public ApplicationLoop
    {
    public:
        CoreApplication(const ApplicationDescription& description);
        virtual ~CoreApplication();

        bool is_running() const;
        bool is_marked_for_destruction() const;

        int32 run();
        void quit();

    protected:
        void mark_for_destroy();

        void initialize();
        void update();
        void shutdown();

        virtual void on_initialize() { /**/ }
        virtual void on_update(const FrameInfo& /*info*/) { /**/ }
        virtual void on_shutdown() { /**/ }

    private:
        void loop() final;

        void create_display_manager();
        void create_window();

        static CoreApplication* s_instance;
        
        bool m_is_marked_for_destruction;
        bool m_is_running;

        ApplicationDescription m_description;

        std::unique_ptr<DisplayManager> m_display_manager;
        std::unique_ptr<Window> m_window;
    };

    // This will be implemented by the CLIENT
    CoreApplication* create_application(const ApplicationArguments& arguments);
}