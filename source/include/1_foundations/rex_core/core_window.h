#pragma once

#include "event.h"

#include "core_window_flags.h"

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
            , flags()
        {
        }

        const char* title;
        int32 width;
        int32 height;
        bool fullscreen;
        const Display* display;
        uint32 display_mode_index;
        WindowFlags flags;
        events::EventCallbackFn event_callback;
    };

    struct Focus
    {
        enum class Type
        {
            KEYBOARD,
            MOUSE
        };

        Focus::Type type;
        bool acquired;
    };

    class CoreWindow
    {
    public:
        virtual ~CoreWindow() = default;

        virtual void update() = 0;

        virtual void show() = 0;
        virtual void hide() = 0;

        virtual void gained_focus(const Focus::Type& focus) = 0;
        virtual void lost_focus(const Focus::Type& focus) = 0;

        virtual bool is_visible() const = 0;
        virtual bool is_windowed() const = 0;
        virtual bool is_fullscreen() const = 0;

        virtual bool set_title(const StringID& title) = 0;
        virtual bool set_windowed() = 0;
        virtual bool set_fullscreen(const Display* display = nullptr, int32 displayModeIndex = 0) = 0;

        virtual int32 get_width() const = 0;
        virtual int32 get_height() const = 0;
    };
}