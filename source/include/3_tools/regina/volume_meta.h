#pragma once

#include "min_max.h"

namespace regina
{
    struct VolumeMeta
    {
        VolumeMeta();
        VolumeMeta(const VolumeMeta& other);
        VolumeMeta(VolumeMeta&& other) noexcept;
        VolumeMeta(const rex::vec3& voxelGridSize, const rex::vec3& voxelGridCellSize, const rex::AABB& voxelGridBounds);

        VolumeMeta& operator=(const VolumeMeta& other);
        VolumeMeta& operator=(VolumeMeta&& other) noexcept;
        
        rex::vec3 voxel_grid_size;
        rex::vec3 voxel_grid_cell_size;
        rex::AABB voxel_grid_bounds;
    };
}