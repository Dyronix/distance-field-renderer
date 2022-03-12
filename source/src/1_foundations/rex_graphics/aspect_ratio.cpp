#include "rex_graphics_pch.h"

#include "aspect_ratio.h"

namespace rex
{
    //-------------------------------------------------------------------------
    AspectRatio::AspectRatio(float width, float height)
        : m_width(width)
        , m_height(height)
        , m_ratio(width, height)
    {
    }
    //-------------------------------------------------------------------------
    void AspectRatio::set_width(float width)
    {
        m_width = width;
        m_ratio = Ratio(m_width, m_height);
    }
    //-------------------------------------------------------------------------
    void AspectRatio::set_height(float height)
    {
        m_height = height;
        m_ratio = Ratio(m_width, m_height);
    }

    //-------------------------------------------------------------------------
    float AspectRatio::get_width() const
    {
        return m_width;
    }
    //-------------------------------------------------------------------------
    float AspectRatio::get_height() const
    {
        return m_height;
    }

    //-------------------------------------------------------------------------
    AspectRatio::Ratio AspectRatio::get_ratio() const
    {
        return m_ratio;
    }
}