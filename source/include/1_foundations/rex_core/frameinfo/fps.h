#pragma once

#include "timing/interval.h"

#include <chrono>

using namespace std::chrono_literals;

namespace rex
{
    class FPS
    {
    public:
        //--------------------------------------------------------------------------------------------
        FPS()
            : m_fps(0)
            , m_fps_count(0)
        {
        }

        //--------------------------------------------------------------------------------------------
        void update()
        {
            ++m_fps_count;

            // one second elapsed? (= 1000 milliseconds)
            if (m_fps_interval.value() > (unsigned)std::chrono::duration_cast<std::chrono::milliseconds>(1s).count())
            {
                m_fps = m_fps_count;

                m_fps_count = 0;
                m_fps_interval = Interval();
            }
        }

        //--------------------------------------------------------------------------------------------
        uint32 get() const
        {
            return m_fps;
        }

    private:
        uint32 m_fps;
        uint32 m_fps_count;

        Interval m_fps_interval;
    };
}