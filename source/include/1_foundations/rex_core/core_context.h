#pragma once

namespace rex
{
    enum class SwapInterval
    {
        IMMEDIATE_UPDATE = 0,
        SYNCHRONIZED_VERTICAL_UPDATE = 1,
        ADAPTIVE_VERTICAL_UPDATE = -1
    };

    class CoreContext
    {
    public:
        virtual ~CoreContext() = default;

        virtual void* get_sdl_context() const = 0;

        virtual const char* get_version() const = 0;
        virtual const char* get_renderer() const = 0;
        virtual const char* get_vendor() const = 0;

        virtual void set_vertical_update(const SwapInterval& interval) = 0;

        virtual void make_current() = 0;
        virtual void present() = 0;
    };
}