#include "regina_pch.h"

#include "lattice_meta.h"

namespace regina
{
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta()
        : lattice_strud_thickness(0.0f)
        , lattice_grid_size(rex::zero_vec3<float>())
        , lattice_grid_cell_size(rex::zero_vec3<float>())
        , voxel_grid_size()
        , voxel_grid_cell_size()
        , voxel_grid_bounds()
    {
    }
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta(const LatticeMeta& other)
        : lattice_strud_thickness(other.lattice_strud_thickness)
        , lattice_grid_size(other.lattice_grid_size)
        , lattice_grid_cell_size(other.lattice_grid_cell_size)
        , voxel_grid_size(other.voxel_grid_size)
        , voxel_grid_cell_size(other.voxel_grid_cell_size)
        , voxel_grid_bounds(other.voxel_grid_bounds)
    {
    }
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta(LatticeMeta&& other) noexcept
        : lattice_strud_thickness(std::exchange(other.lattice_strud_thickness, 0.0f))
        , lattice_grid_size(std::exchange(other.lattice_grid_size, rex::zero_vec3<float>()))
        , lattice_grid_cell_size(std::exchange(other.lattice_grid_cell_size, rex::zero_vec3<float>()))
        , voxel_grid_size(std::exchange(other.voxel_grid_size, {}))
        , voxel_grid_cell_size(std::exchange(other.voxel_grid_cell_size, {}))
        , voxel_grid_bounds(std::exchange(other.voxel_grid_bounds, {}))
    {
    }
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta(float strudThickness, const rex::vec3& latticeGridSize, const rex::vec3& latticeGridCellSize, const rex::AABB& voxelGridBounds, const rex::vec3& voxelGridSize, const rex::vec3& voxelGridCellSize)
        : lattice_strud_thickness(strudThickness)
        , lattice_grid_size(latticeGridSize)
        , lattice_grid_cell_size(latticeGridCellSize)
        , voxel_grid_size(voxelGridSize)
        , voxel_grid_cell_size(voxelGridCellSize)
        , voxel_grid_bounds(voxelGridBounds)
    {
    }

    //-------------------------------------------------------------------------
    LatticeMeta& LatticeMeta::operator=(const LatticeMeta& other)
    {
        lattice_strud_thickness = other.lattice_strud_thickness;
        lattice_grid_size = other.lattice_grid_size;
        lattice_grid_cell_size = other.lattice_grid_cell_size;
        voxel_grid_size = other.voxel_grid_size;
        voxel_grid_cell_size = other.voxel_grid_cell_size;
        voxel_grid_bounds = other.voxel_grid_bounds;

        return *this;
    }
    //-------------------------------------------------------------------------
    LatticeMeta& LatticeMeta::operator=(LatticeMeta&& other) noexcept
    {
        lattice_strud_thickness = std::exchange(other.lattice_strud_thickness, 0.0f);
        lattice_grid_size = std::exchange(other.lattice_grid_size, rex::zero_vec3<float>());
        lattice_grid_cell_size = std::exchange(other.lattice_grid_cell_size, rex::zero_vec3<float>());
        voxel_grid_size = std::exchange(other.voxel_grid_size, {});
        voxel_grid_cell_size = std::exchange(other.voxel_grid_cell_size, {});
        voxel_grid_bounds = std::exchange(other.voxel_grid_bounds, {});

        return *this;
    }
} // namespace regina