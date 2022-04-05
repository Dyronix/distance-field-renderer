#pragma once 

namespace rex
{
    class Texture;
}

namespace regina
{
    struct VolumeMeta;

    class Volume;

    namespace volume_library
    {
        bool add(Volume&& volume);

        Volume& get_volume(const rex::StringID& volumeName);
        rex::ref_ptr<rex::Texture>& get_volume_data(const rex::StringID& volumeName);

        void clear();
    }
}