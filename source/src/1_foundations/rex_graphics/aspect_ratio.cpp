#include "rex_graphics_pch.h"

#include "aspectratio.h"

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
    void AspectRatio::setWidth(float width)
    {
        m_width = width;
        m_ratio = Ratio(m_width, m_height);
    }
    //-------------------------------------------------------------------------
    void AspectRatio::setHeight(float height)
    {
        m_height = height;
        m_ratio = Ratio(m_width, m_height);
    }

    //-------------------------------------------------------------------------
    float AspectRatio::getWidth() const
    {
        return m_width;
    }
    //-------------------------------------------------------------------------
    float AspectRatio::getHeight() const
    {
        return m_height;
    }

    //-------------------------------------------------------------------------
    AspectRatio::Ratio AspectRatio::getRatio() const
    {
        return m_ratio;
    }
}