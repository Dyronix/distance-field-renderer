#pragma once

#include "timing/interval.h"

#include <chrono>

using namespace std::chrono_literals;

namespace rex
{
    class FPS
    {
    public:
        FPS();
        FPS(const FPS& other);
        FPS(FPS&& other);

        FPS& operator=(const FPS& other);
        FPS& operator=(FPS&& other);

        operator uint32() const;

        void update();

        uint32 get() const;

    private:
        uint32 m_fps;
        uint32 m_fps_count;

        Interval m_fps_interval;
    };
}