#include "regina_pch.h"

#include "volume_importer.h"

#include "volume_meta.h"
#include "volume_meta_parser.h"

#include "file_import.h"

namespace regina
{
    namespace volume_importer
    {
        //-------------------------------------------------------------------------
        Volume import(const rex::StringID& name, const rex::StringID& volumeMetaPath, const rex::StringID& volumeDataPath)
        {   
            R_INFO("[VOLUME] Loading Volume: {0}", name.to_string());

            rex::FileMemory volume_meta_input = rex::read_from_disk(volumeMetaPath.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);
            rex::FileMemory volume_data_input = rex::read_from_disk(volumeDataPath.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);

            VolumeMetaParser parser;
            VolumeMeta meta = parser.parse(volume_meta_input);

            return Volume(name, meta, std::move(volume_data_input));
        }
    }
}