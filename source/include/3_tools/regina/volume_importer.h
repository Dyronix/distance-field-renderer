#pragma once

#include "volume.h"

namespace regina
{
    namespace volume_importer
    {
        Volume import(const rex::StringID& name, const rex::StringID& volumeMetaPath, const rex::StringID& volumeDataPath);
    }
}