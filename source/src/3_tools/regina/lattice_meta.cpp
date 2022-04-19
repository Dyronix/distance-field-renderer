#include "regina_pch.h"

#include "lattice_meta.h"

namespace regina
{
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta()
        : lattice_strud_thickness(0.0f)
        , lattice_grid_size(rex::zero_vec3<float>())
        , lattice_grid_cell_size(rex::zero_vec3<float>())
        , lattice_grid_bounds()
    {
    }
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta(const LatticeMeta& other)
        : lattice_strud_thickness(other.lattice_strud_thickness)
        , lattice_grid_size(other.lattice_grid_size)
        , lattice_grid_cell_size(other.lattice_grid_cell_size)
        , lattice_grid_bounds(other.lattice_grid_bounds)
    {
    }
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta(LatticeMeta&& other) noexcept
        : lattice_strud_thickness(std::exchange(other.lattice_strud_thickness, 0.0f))
        , lattice_grid_size(std::exchange(other.lattice_grid_size, rex::zero_vec3<float>()))
        , lattice_grid_cell_size(std::exchange(other.lattice_grid_cell_size, rex::zero_vec3<float>()))
        , lattice_grid_bounds(std::exchange(other.lattice_grid_bounds, {}))
    {
    }
    //-------------------------------------------------------------------------
    LatticeMeta::LatticeMeta(float strudThickness, const rex::vec3& latticeGridSize, const rex::vec3& latticeGridCellSize, const rex::AABB& latticeGridBounds)
        : lattice_strud_thickness(strudThickness)
        , lattice_grid_size(latticeGridSize)
        , lattice_grid_cell_size(latticeGridCellSize)
        , lattice_grid_bounds(latticeGridBounds)
    {
    }

    //-------------------------------------------------------------------------
    LatticeMeta& LatticeMeta::operator=(const LatticeMeta& other)
    {
        lattice_strud_thickness = other.lattice_strud_thickness;
        lattice_grid_size = other.lattice_grid_size;
        lattice_grid_cell_size = other.lattice_grid_cell_size;
        lattice_grid_bounds = other.lattice_grid_bounds;

        return *this;
    }
    //-------------------------------------------------------------------------
    LatticeMeta& LatticeMeta::operator=(LatticeMeta&& other) noexcept
    {
        lattice_strud_thickness = std::exchange(other.lattice_strud_thickness, 0.0f);
        lattice_grid_size = std::exchange(other.lattice_grid_size, rex::zero_vec3<float>());
        lattice_grid_cell_size = std::exchange(other.lattice_grid_cell_size, rex::zero_vec3<float>());
        lattice_grid_bounds = std::exchange(other.lattice_grid_bounds, {});

        return *this;
    }
} // namespace regina