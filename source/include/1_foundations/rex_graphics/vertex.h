#pragma once

namespace rex
{
    struct Vertex
    {
        rex::vec3 position;
        rex::vec3 normal;
        rex::vec3 tangent;
        rex::vec3 binormal;
        rex::vec2 texcoord;
    };
}