#include "rex_graphics_pch.h"

#include "fieldofview.h"

namespace rex
{
    FieldOfView::FieldOfView(const DegAngle& horizontal, const DegAngle& vertical)
    {
        setHorizontal(horizontal);
        setVertical(vertical);
    }

    //-------------------------------------------------------------------------
    FieldOfView::FieldOfView(const DegAngle& vertical, const AspectRatio::Ratio& aspectRatio)
    {
        setVertical(vertical, aspectRatio);
    }

    //-------------------------------------------------------------------------
    void FieldOfView::reevaluate(const AspectRatio::Ratio& aspectRatio)
    {
        setHorizontal(getHorizontal(), aspectRatio);
        setVertical(getVertical(), aspectRatio);
    }

    //-------------------------------------------------------------------------
    void FieldOfView::setHorizontal(const DegAngle& h)
    {
        m_horizontal = h;
    }
    //-------------------------------------------------------------------------
    void FieldOfView::setHorizontal(const DegAngle& h, const AspectRatio::Ratio& aspectRatio)
    {
        R_ASSERT(aspectRatio.get() != 0);

        m_horizontal = h;
        m_vertical = DegAngle(rad2deg(2.0f * std::atan(std::tan(m_horizontal.to_rad() * 0.5f) / aspectRatio.get())));
    }

    //-------------------------------------------------------------------------
    void FieldOfView::setVertical(const DegAngle& v)
    {
        m_vertical = v;
    }
    //-------------------------------------------------------------------------
    void FieldOfView::setVertical(const DegAngle& v, const AspectRatio::Ratio& aspectRatio)
    {
        m_vertical = v;
        m_horizontal = DegAngle(rad2deg(2.0f * std::atan(aspectRatio.get() * std::tan(m_vertical.to_rad() * 0.5f))));
    }

    //-------------------------------------------------------------------------
    const DegAngle& FieldOfView::getHorizontal() const
    {
        return m_horizontal;
    }
    //-------------------------------------------------------------------------
    const DegAngle& FieldOfView::getVertical() const
    {
        return m_vertical;
    }
}