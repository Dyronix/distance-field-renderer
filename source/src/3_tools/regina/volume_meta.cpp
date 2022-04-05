#include "regina_pch.h"

#include "volume_meta.h"

namespace regina
{
    //-------------------------------------------------------------------------
    VolumeMeta::VolumeMeta()
        :voxel_grid_size(rex::zero_vec3<float>())
        ,voxel_grid_cell_size(rex::zero_vec3<float>())
        ,voxel_grid_bounds()
    {

    }
    //-------------------------------------------------------------------------
    VolumeMeta::VolumeMeta(const VolumeMeta& other)
        :voxel_grid_size(other.voxel_grid_size)
        ,voxel_grid_cell_size(other.voxel_grid_cell_size)
        ,voxel_grid_bounds(other.voxel_grid_bounds)
    {

    }
    //-------------------------------------------------------------------------
    VolumeMeta::VolumeMeta(VolumeMeta&& other) noexcept
        :voxel_grid_size(std::exchange(other.voxel_grid_size, rex::zero_vec3<float>()))
        ,voxel_grid_cell_size(std::exchange(other.voxel_grid_cell_size, rex::zero_vec3<float>()))
        ,voxel_grid_bounds(std::exchange(other.voxel_grid_bounds, {}))
    {

    }
    //-------------------------------------------------------------------------
    VolumeMeta::VolumeMeta(const rex::vec3& voxelGridSize, const rex::vec3& voxelGridCellSize, const rex::AABB& voxelGridBounds)
        :voxel_grid_size(voxelGridSize)
        ,voxel_grid_cell_size(voxelGridCellSize)
        ,voxel_grid_bounds(voxelGridBounds)
    {

    }

    //-------------------------------------------------------------------------
    VolumeMeta& VolumeMeta::operator=(const VolumeMeta& other)
    {
        voxel_grid_size = other.voxel_grid_size;
        voxel_grid_cell_size = other.voxel_grid_cell_size;
        voxel_grid_bounds = other.voxel_grid_bounds;

        return *this;
    }
    //-------------------------------------------------------------------------
    VolumeMeta& VolumeMeta::operator=(VolumeMeta&& other) noexcept
    {
        voxel_grid_size = std::exchange(other.voxel_grid_size, rex::zero_vec3<float>());
        voxel_grid_cell_size = std::exchange(other.voxel_grid_cell_size, rex::zero_vec3<float>());
        voxel_grid_bounds = std::exchange(other.voxel_grid_bounds, {});

        return *this;
    }
}