#include "rex_graphics_pch.h"

#include "field_of_view.h"

#include "rad_angle.h"

namespace rex
{
    FieldOfView::FieldOfView(const DegAngle& horizontal, const DegAngle& vertical)
    {
        set_horizontal(horizontal);
        set_vertical(vertical);
    }

    //-------------------------------------------------------------------------
    FieldOfView::FieldOfView(const DegAngle& vertical, const AspectRatio::Ratio& aspectRatio)
    {
        set_vertical(vertical, aspectRatio);
    }

    //-------------------------------------------------------------------------
    void FieldOfView::reevaluate(const AspectRatio::Ratio& aspectRatio)
    {
        set_horizontal(get_horizontal(), aspectRatio);
        set_vertical(get_vertical(), aspectRatio);
    }

    //-------------------------------------------------------------------------
    void FieldOfView::set_horizontal(const DegAngle& h)
    {
        m_horizontal = h;
    }
    //-------------------------------------------------------------------------
    void FieldOfView::set_horizontal(const DegAngle& h, const AspectRatio::Ratio& aspectRatio)
    {
        R_ASSERT(aspectRatio.get() != 0);

        m_horizontal = h;
        m_vertical = DegAngle(rad2deg(2.0f * std::atan(std::tan(m_horizontal.to_rad() * 0.5f) / aspectRatio.get())));
    }

    //-------------------------------------------------------------------------
    void FieldOfView::set_vertical(const DegAngle& v)
    {
        m_vertical = v;
    }
    //-------------------------------------------------------------------------
    void FieldOfView::set_vertical(const DegAngle& v, const AspectRatio::Ratio& aspectRatio)
    {
        m_vertical = v;
        m_horizontal = DegAngle(rad2deg(2.0f * std::atan(aspectRatio.get() * std::tan(m_vertical.to_rad() * 0.5f))));
    }

    //-------------------------------------------------------------------------
    const DegAngle& FieldOfView::get_horizontal() const
    {
        return m_horizontal;
    }
    //-------------------------------------------------------------------------
    const DegAngle& FieldOfView::get_vertical() const
    {
        return m_vertical;
    }
}