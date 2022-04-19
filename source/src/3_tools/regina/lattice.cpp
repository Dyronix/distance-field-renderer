#include "regina_pch.h"

#include "lattice.h"

namespace regina
{
    //-------------------------------------------------------------------------
    Lattice::Lattice()
        : m_lattice_name(rex::ESID::SID_None)
        , m_lattice_meta()
        , m_lattice_data(nullptr)
    {
    }
    //-------------------------------------------------------------------------
    Lattice::Lattice(const Lattice& other)
        : m_lattice_name(other.m_lattice_name)
        , m_lattice_meta(other.m_lattice_meta)
        , m_lattice_data(other.m_lattice_data)
    {
    }
    //-------------------------------------------------------------------------
    Lattice::Lattice(Lattice&& other) noexcept
        : m_lattice_name(std::exchange(other.m_lattice_name, rex::ESID::SID_None))
        , m_lattice_meta(std::exchange(other.m_lattice_meta, {}))
        , m_lattice_data(std::exchange(other.m_lattice_data, nullptr))
    {
    }
    //-------------------------------------------------------------------------
    Lattice::Lattice(const rex::StringID& latticeName, const LatticeMeta& latticeMeta, rex::memory::Blob&& latticeData)
        : m_lattice_name(latticeName)
        , m_lattice_meta(latticeMeta)
        , m_lattice_data(std::move(latticeData))
    {
    }

    //-------------------------------------------------------------------------
    Lattice& Lattice::operator=(const Lattice& other)
    {
        m_lattice_name = other.m_lattice_name;
        m_lattice_meta = other.m_lattice_meta;
        m_lattice_data = other.m_lattice_data;

        return *this;
    }
    //-------------------------------------------------------------------------
    Lattice& Lattice::operator=(Lattice&& other) noexcept
    {
        m_lattice_name = std::exchange(other.m_lattice_name, rex::ESID::SID_None);
        m_lattice_meta = std::exchange(other.m_lattice_meta, {});
        m_lattice_data = std::exchange(other.m_lattice_data, nullptr);

        return *this;
    }

    //-------------------------------------------------------------------------
    const rex::StringID& Lattice::get_name() const
    {
        return m_lattice_name;
    }

    //-------------------------------------------------------------------------
    float Lattice::get_lattice_strud_thickness() const
    {
        return m_lattice_meta.lattice_strud_thickness;
    }
    //-------------------------------------------------------------------------
    const rex::vec3& Lattice::get_lattice_grid_cell_size() const
    {
        return m_lattice_meta.lattice_grid_cell_size;
    }
    //-------------------------------------------------------------------------
    const rex::vec3& Lattice::get_lattice_grid_size() const
    {
        return m_lattice_meta.lattice_grid_size;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& Lattice::get_voxel_grid_cell_size() const
    {
        return m_lattice_meta.voxel_grid_cell_size;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& Lattice::get_voxel_grid_size() const
    {
        return m_lattice_meta.voxel_grid_cell_size;
    }

    //-------------------------------------------------------------------------
    const rex::AABB& Lattice::get_voxel_grid_bounds() const
    {
        return m_lattice_meta.voxel_grid_bounds;
    }

    //-------------------------------------------------------------------------
    const LatticeMeta& Lattice::get_lattice_meta() const
    {
        return m_lattice_meta;
    }
    //-------------------------------------------------------------------------
    const rex::memory::Blob& Lattice::get_lattice_data() const
    {
        return m_lattice_data;
    }
} // namespace regina