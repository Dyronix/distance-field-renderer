#include "rex_graphics_pch.h"

#include "triangle.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
        : v1(v1)
        , v2(v2)
        , v3(v3)
    {
    }
}