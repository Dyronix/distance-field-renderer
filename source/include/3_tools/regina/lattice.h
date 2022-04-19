#pragma once

#include "blob.h"

#include "lattice_meta.h"

namespace regina
{
    class Lattice
    {
    public:
        Lattice();
        Lattice(const Lattice& other);
        Lattice(Lattice&& other) noexcept;
        Lattice(const rex::StringID& latticeName, const LatticeMeta& latticeMeta, rex::memory::Blob&& latticeData);

        Lattice& operator=(const Lattice& other);
        Lattice& operator=(Lattice&& other) noexcept;

        const rex::StringID& get_name() const;

        float get_lattice_strud_thickness() const;
        const rex::vec3& get_lattice_grid_cell_size() const;
        const rex::vec3& get_lattice_grid_size() const;
        const rex::vec3& get_voxel_grid_cell_size() const;
        const rex::vec3& get_voxel_grid_size() const;
        const rex::AABB& get_voxel_grid_bounds() const;

        const LatticeMeta& get_lattice_meta() const;
        const rex::memory::Blob& get_lattice_data() const;

    private:
        rex::StringID m_lattice_name;
        rex::memory::Blob m_lattice_data;

        LatticeMeta m_lattice_meta;
    };
} // namespace regina