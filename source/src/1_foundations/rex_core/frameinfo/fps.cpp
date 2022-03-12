#include "rex_utilities_pch.h"

#include "frameinfo/fps.h"

namespace rex
{
    //--------------------------------------------------------------------------------------------
    FPS::FPS()
        : m_fps(0)
        , m_fps_count(0)
    {
    }
    //--------------------------------------------------------------------------------------------
    FPS::FPS(const FPS& other)
        : m_fps(other.m_fps)
        , m_fps_count(other.m_fps_count)
    {
    }
    //--------------------------------------------------------------------------------------------
    FPS::FPS(FPS&& other)
        : m_fps(std::exchange(other.m_fps, {}))
        , m_fps_count(std::exchange(other.m_fps_count, {}))
    {
    }

    //--------------------------------------------------------------------------------------------
    FPS& FPS::operator=(const FPS& other)
    {
        m_fps = other.m_fps;
        m_fps_count = other.m_fps_count;

        return *this;
    }
    //--------------------------------------------------------------------------------------------
    FPS& FPS::operator=(FPS&& other)
    {
        m_fps = std::exchange(other.m_fps, {});
        m_fps_count = std::exchange(other.m_fps_count, {});

        return *this;
    }

    //--------------------------------------------------------------------------------------------
    FPS::operator uint32() const
    {
        return m_fps;
    }

    //--------------------------------------------------------------------------------------------
    void FPS::update()
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
    uint32 FPS::get() const
    {
        return m_fps;
    }
}