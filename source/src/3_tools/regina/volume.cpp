#include "regina_pch.h"

#include "volume.h"

namespace regina
{
    //-------------------------------------------------------------------------
    Volume::Volume()
        :m_volume_name(rex::ESID::SID_None)
        ,m_volume_meta()
        ,m_volume_data(nullptr)
    {

    }
    //-------------------------------------------------------------------------
    Volume::Volume(const Volume& other)
        :m_volume_name(other.m_volume_name)
        ,m_volume_meta(other.m_volume_meta)
        ,m_volume_data(other.m_volume_data)
    {

    }
    //-------------------------------------------------------------------------
    Volume::Volume(Volume&& other) noexcept
        :m_volume_name(std::exchange(other.m_volume_name, rex::ESID::SID_None))
        ,m_volume_meta(std::exchange(other.m_volume_meta, {}))
        ,m_volume_data(std::exchange(other.m_volume_data, nullptr))
    {

    }
    //-------------------------------------------------------------------------
    Volume::Volume(const rex::StringID& volumeName, const VolumeMeta& volumeMeta, rex::memory::Blob&& volumeData)
        :m_volume_name(volumeName)
        ,m_volume_meta(volumeMeta)
        ,m_volume_data(std::move(volumeData))
    {

    }

    //-------------------------------------------------------------------------
    Volume& Volume::operator=(const Volume& other)
    {
        m_volume_name = other.m_volume_name;
        m_volume_meta = other.m_volume_meta;
        m_volume_data = other.m_volume_data;

        return *this;
    }
    //-------------------------------------------------------------------------
    Volume& Volume::operator=(Volume&& other) noexcept
    {
        m_volume_name = std::exchange(other.m_volume_name, rex::ESID::SID_None);
        m_volume_meta = std::exchange(other.m_volume_meta, {});
        m_volume_data = std::exchange(other.m_volume_data, nullptr);

        return *this;
    }

    //-------------------------------------------------------------------------
    const rex::StringID& Volume::get_name() const
    {
        return m_volume_name;
    }
    //-------------------------------------------------------------------------
    const rex::vec3& Volume::get_voxel_grid_cell_size() const
    {
        return m_volume_meta.voxel_grid_cell_size;
    }
    //-------------------------------------------------------------------------
    const rex::vec3& Volume::get_voxel_grid_size() const
    {
        return m_volume_meta.voxel_grid_size;
    }
    //-------------------------------------------------------------------------
    const rex::AABB& Volume::get_voxel_grid_bounds() const
    {
        return m_volume_meta.voxel_grid_bounds;
    }
    //-------------------------------------------------------------------------
    const VolumeMeta& Volume::get_volume_meta() const
    {
        return m_volume_meta;
    }
    //-------------------------------------------------------------------------
    const rex::memory::Blob& Volume::get_volume_data() const
    {
        return m_volume_data;
    }
}