#pragma once

namespace rex
{
    class WindowFlags
    {
    public:
        enum class Flags
        {
            NONE = 0,  // no flags

            FULLSCREEN = BIT(0),          // fullscreen window
            FULLSCREEN_DESKTOP = BIT(1),  // fullscreen window at desktop resolution

            HIDDEN = BIT(2),      //  window is not visible
            BORDERLESS = BIT(3),  //  no window decoration
            RESIZABLE = BIT(4),   //  window can be resized
            MINIMIZED = BIT(5),   //  window is minimized
            MAXIMIZED = BIT(6),   //  window is maximized

            INPUT_GRABBED = BIT(7),  //  window has grabbed input focus
            ALLOW_HIGHDPI = BIT(8),  //  window should be created in high-DPI mode if supported

            V_SYNC = BIT(9)  //  window should enable v sync
        };

        WindowFlags();
        WindowFlags(WindowFlags::Flags f);
        ~WindowFlags();

        WindowFlags::Flags get() const;

    private:
        WindowFlags::Flags m_flags;
    };

    namespace conversions
    {
        std::string to_display_string(const WindowFlags& flags);
        std::string to_display_string(const WindowFlags::Flags& flags);
    }
}