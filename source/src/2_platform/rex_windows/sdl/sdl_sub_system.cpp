#include "rex_windows_pch.h"

#include "sdl/sdl_sub_system.h"

#include <SDL_syswm.h>

namespace rex
{
    namespace sdl
    {
        //-------------------------------------------------------------------------
        StringID convert_to_sdl_subsystem_string(SDL_Window* window, SDL_SysWMinfo* info)
        {
            if (!SDL_GetWindowWMInfo(window, info))
            {
                return "Unknown";
            }

            SDL_SYSWM_TYPE system_type = info->subsystem;

            switch (system_type)
            {
                case SDL_SYSWM_UNKNOWN: return "Unknown"_sid;
                case SDL_SYSWM_WINDOWS: return "Microsoft Windows"_sid;
                case SDL_SYSWM_X11: return "X Window System"_sid;
                case SDL_SYSWM_DIRECTFB: return "DirectFB"_sid;
                case SDL_SYSWM_COCOA: return "Apple Mac OS X"_sid;
                case SDL_SYSWM_UIKIT: return "Apple iOS"_sid;
                default:  // __clang__
                    break;
            }

            SDL_version sdl_linked_version;
            SDL_GetVersion(&sdl_linked_version);

            if (sdl_linked_version.major >= 2 && sdl_linked_version.patch >= 2)
            {
                switch (system_type)
                {
                    case SDL_SYSWM_WAYLAND: return "Wayland"_sid;
                    case SDL_SYSWM_MIR: return "Mir"_sid;
                    default:  // __clang__
                        break;
                }

                if (sdl_linked_version.patch >= 3)
                {
                    switch (system_type)
                    {
                        case SDL_SYSWM_WINRT: return "WinRT"_sid;
                        default:  // __clang__
                            break;
                    }

                    if (sdl_linked_version.patch >= 4)
                    {
                        switch (system_type)
                        {
                            case SDL_SYSWM_ANDROID: return "Android"_sid;
                            default:  // __clang__
                                break;
                        }

                        if (sdl_linked_version.patch >= 5)
                        {
                            switch (system_type)
                            {
                                case SDL_SYSWM_VIVANTE: return "Vivante"_sid;
                                default:  // __clang__
                                    break;
                            }
                        }
                    }
                }
            }

            R_ASSERT_X(false, "Unsupported platform");
            return "Unknown"_sid;
        }
    }
}