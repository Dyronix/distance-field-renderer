#pragma once

namespace rex
{
    template <typename T>
    struct BaseRect
    {
        //-------------------------------------------------------------------------
        BaseRect()
            : x(T())
            , y(T())
            , width(T())
            , height(T())
        {
        }
        //-------------------------------------------------------------------------
        BaseRect(T inX, T inY, T inWidth, T inHeight)
            : x(inX)
            , y(inY)
            , width(inWidth)
            , height(inHeight)
        {
        }

        T x;
        T y;
        T width;
        T height;
    };

    using RectI = BaseRect<int>;
    using RectF = BaseRect<float>;
    using RectD = BaseRect<double>;

    using Rect = RectF;
}