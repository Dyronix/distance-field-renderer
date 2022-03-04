#pragma once 

struct SDL_Window;

namespace rex
{
    class Display;

    struct WindowDescription
    {
        WindowDescription()
            :title("None")
            ,width(-1)
            ,height(-1)
            ,fullscreen(false)
            ,display(nullptr)
            ,display_mode_index(0)
        {}

        const char* title;
        int32 width;
        int32 height;
        bool fullscreen;
        const Display* display;
        uint32 display_mode_index;
    };

    class Window
    {
    public:
        Window(const WindowDescription& description);
        ~Window();

        void update();

        void show();
        void hide();

        bool set_windowed();
        bool set_fullscreen(const Display* display = nullptr);

        int32 get_width() const;
        int32 get_height() const;

    private:
        void set_display_mode(const gsl::not_null<const Display*> display, int32 displayModeIndex = 0);

        SDL_Window* m_sdl_window;

        int32 m_windowed_width;
        int32 m_windowed_height;
    };
}