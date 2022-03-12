#pragma once

#include "min_max.h"

namespace rex
{
    struct Submesh
    {
        uint32 base_vertex;
        uint32 base_index;

        uint32 material_index;

        uint32 index_count;
        uint32 vertex_count;

        rex::matrix4 transform;

        AABB bounding_box;

        StringID node_name;
        StringID mesh_name;
    };
}