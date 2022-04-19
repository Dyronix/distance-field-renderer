#pragma once

#include "min_max.h"

namespace regina
{
    struct LatticeMeta
    {
        LatticeMeta();
        LatticeMeta(const LatticeMeta& other);
        LatticeMeta(LatticeMeta&& other) noexcept;
        LatticeMeta(float strudThickness, const rex::vec3& latticeGridSize, const rex::vec3& latticeGridCellSize, const rex::AABB& latticeGridBounds);

        LatticeMeta& operator=(const LatticeMeta& other);
        LatticeMeta& operator=(LatticeMeta&& other) noexcept;

        float lattice_strud_thickness;

        rex::vec3 lattice_grid_size;
        rex::vec3 lattice_grid_cell_size;
        rex::AABB lattice_grid_bounds;
    };
} // namespace regina