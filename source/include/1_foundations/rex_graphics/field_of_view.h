#pragma once

#include "degangle.h"
#include "degradians.h"

#include "aspectratio.h"

namespace rex
{
    class DegAngle;

    class FieldOfView
    {
    public:
        FieldOfView(const DegAngle& horizontal, const DegAngle& vertical);
        FieldOfView(const DegAngle& vertical, const AspectRatio::Ratio& aspectRatio);

        void reevaluate(const AspectRatio::Ratio& aspectRatio);

        void setHorizontal(const DegAngle& h);
        void setHorizontal(const DegAngle& h, const AspectRatio::Ratio& aspectRatio);
        void setVertical(const DegAngle& v);
        void setVertical(const DegAngle& v, const AspectRatio::Ratio& aspectRatio);

        const DegAngle& getHorizontal() const;
        const DegAngle& getVertical() const;

    private:
        DegAngle m_horizontal;
        DegAngle m_vertical;
    };
}