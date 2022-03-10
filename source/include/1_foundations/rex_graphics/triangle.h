#pragma once

#include "vertex.h"

namespace rex
{
    struct TriangleIndices
    {
        static constexpr uint32 INDICES_PER_TRIANGLE = 3u;

        uint32 v1;
        uint32 v2;
        uint32 v3;
    };

    struct Triangle
    {
        Vertex v1, v2, v3;

        Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
    };
}