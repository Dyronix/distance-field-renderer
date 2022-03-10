#pragma once

namespace rex
{
    class Display;

    struct WindowDescription
    {
        WindowDescription()
            : title("None")
            , width(-1)
            , height(-1)
            , fullscreen(false)
            , display(nullptr)
            , display_mode_index(0)
        {
        }

        const char* title;
        int32 width;
        int32 height;
        bool fullscreen;
        const Display* display;
        uint32 display_mode_index;
    };

    class CoreWindow
    {
    public:
        virtual ~CoreWindow() = default;

        virtual void update() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;

        virtual bool set_windowed() = 0;
        virtual bool set_fullscreen(const Display* display = nullptr) = 0;

        virtual int32 get_width() const = 0;
        virtual int32 get_height() const = 0;
    };
}