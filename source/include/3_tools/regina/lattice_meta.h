#pragma once

#include "min_max.h"

namespace regina
{
    struct LatticeMeta
    {
        LatticeMeta();
        LatticeMeta(const LatticeMeta& other);
        LatticeMeta(LatticeMeta&& other) noexcept;
        LatticeMeta(float strudThickness, const rex::vec3& latticeGridSize, const rex::vec3& latticeGridCellSize, const rex::AABB& voxelGridBounds, const rex::vec3& voxelGridSize, const rex::vec3& voxelGridCellSize);

        LatticeMeta& operator=(const LatticeMeta& other);
        LatticeMeta& operator=(LatticeMeta&& other) noexcept;

        float lattice_strud_thickness;

        rex::vec3 voxel_grid_size;
        rex::vec3 voxel_grid_cell_size;
        rex::AABB voxel_grid_bounds;

        rex::vec3 lattice_grid_size;
        rex::vec3 lattice_grid_cell_size;
    };
} // namespace regina