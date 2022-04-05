#pragma once

#include "blob.h"

#include "volume_meta.h"

namespace regina
{
    class Volume
    {
    public:
        Volume();
        Volume(const Volume& other);
        Volume(Volume&& other) noexcept;
        Volume(const rex::StringID& volumeName, const VolumeMeta& volumeMeta, rex::memory::Blob&& volumeData);

        Volume& operator=(const Volume& other);
        Volume& operator=(Volume&& other) noexcept;

        const rex::StringID& get_name() const;

        const rex::vec3& get_voxel_grid_cell_size() const;
        const rex::vec3& get_voxel_grid_size() const;
        const rex::AABB& get_voxel_grid_bounds() const;

        const VolumeMeta& get_volume_meta() const;
        const rex::memory::Blob& get_volume_data() const;

    private:
        rex::StringID m_volume_name;
        rex::memory::Blob m_volume_data;

        VolumeMeta m_volume_meta;
    };
}