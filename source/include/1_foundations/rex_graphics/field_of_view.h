#pragma once

#include "aspect_ratio.h"

namespace rex
{
    class DegAngle;

    class FieldOfView
    {
    public:
        FieldOfView(const DegAngle& horizontal, const DegAngle& vertical);
        FieldOfView(const DegAngle& vertical, const AspectRatio::Ratio& aspectRatio);

        void reevaluate(const AspectRatio::Ratio& aspectRatio);

        void set_horizontal(const DegAngle& h);
        void set_horizontal(const DegAngle& h, const AspectRatio::Ratio& aspectRatio);
        void set_vertical(const DegAngle& v);
        void set_vertical(const DegAngle& v, const AspectRatio::Ratio& aspectRatio);

        const DegAngle& get_horizontal() const;
        const DegAngle& get_vertical() const;

    private:
        DegAngle m_horizontal;
        DegAngle m_vertical;
    };
}