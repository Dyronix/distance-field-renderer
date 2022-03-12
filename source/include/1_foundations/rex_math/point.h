#pragma once

namespace rex
{
    template<typename T>
    struct BasePoint
    {
        //-------------------------------------------------------------------------
        BasePoint(T xPos = T(), T yPos = T())
            :x(xPos)
            ,y(yPos)
        {}

        T x;
        T y;
    };

    using PointI = BasePoint<int>;
    using PointF = BasePoint<float>;
    using PointD = BasePoint<double>;

    using Point = PointI;
}