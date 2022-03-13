#include "rex_core_pch.h"

#include "core_window_flags.h"

namespace rex
{
    //-------------------------------------------------------------------------
    WindowFlags::WindowFlags()
        : m_flags(WindowFlags::Flags::NONE)
    {
    }
    //-------------------------------------------------------------------------
    WindowFlags::WindowFlags(WindowFlags::Flags f)
        : m_flags(f)
    {
    }
    //-------------------------------------------------------------------------
    WindowFlags::~WindowFlags()
    {
    }

    //-------------------------------------------------------------------------
    WindowFlags::Flags WindowFlags::get() const
    {
        return m_flags;
    }

    namespace conversions
    {
        //-------------------------------------------------------------------------
        std::string to_display_string(const WindowFlags& flags)
        {
            return to_display_string(flags.get());
        }
        //-------------------------------------------------------------------------
        std::string to_display_string(const WindowFlags::Flags& flags)
        {
            std::vector<std::string> s_flags;

            if ((int32)flags & (int32)WindowFlags::Flags::FULLSCREEN)
                s_flags.push_back("FULLSCREEN");
            if ((int32)flags & (int32)WindowFlags::Flags::FULLSCREEN_DESKTOP)
                s_flags.push_back("FULLSCREEN_DESKTOP");

            if ((int32)flags & (int32)WindowFlags::Flags::HIDDEN)
                s_flags.push_back("HIDDEN");
            if ((int32)flags & (int32)WindowFlags::Flags::BORDERLESS)
                s_flags.push_back("BORDERLESS");
            if ((int32)flags & (int32)WindowFlags::Flags::RESIZABLE)
                s_flags.push_back("RESIZABLE");
            if ((int32)flags & (int32)WindowFlags::Flags::MINIMIZED)
                s_flags.push_back("MINIMIZED");
            if ((int32)flags & (int32)WindowFlags::Flags::MAXIMIZED)
                s_flags.push_back("MAXIMIZED");

            if ((int32)flags & (int32)WindowFlags::Flags::INPUT_GRABBED)
                s_flags.push_back("INPUT_GRABBED");
            if ((int32)flags & (int32)WindowFlags::Flags::ALLOW_HIGHDPI)
                s_flags.push_back("ALLOW_HIGHDPI");
            if ((int32)flags & (int32)WindowFlags::Flags::V_SYNC)
                s_flags.push_back("V SYNC");

            std::stringstream stream;

            stream << "Available window flags: ";

            for (int32 i = 0; i < s_flags.size() - 1; ++i)
            {
                stream << s_flags[i] << ", ";
            }

            stream << s_flags.back();
            stream << std::endl;

            return stream.str();
        }
    }
}