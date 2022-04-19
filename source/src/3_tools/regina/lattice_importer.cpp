#include "regina_pch.h"

#include "lattice_importer.h"

#include "lattice_meta.h"
#include "lattice_meta_parser.h"

#include "file_import.h"

namespace regina
{
    namespace lattice_importer
    {
        //-------------------------------------------------------------------------
        Lattice import(const rex::StringID& name, const rex::StringID& latticeMetaPath, const rex::StringID& latticeDataPath)
        {
            R_INFO("[VOLUME] Loading Lattice: {0}", name.to_string());

            rex::FileMemory lattice_meta_input = rex::read_from_disk(latticeMetaPath.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);
            rex::FileMemory lattice_data_input = rex::read_from_disk(latticeDataPath.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);

            LatticeMetaParser parser;
            LatticeMeta meta = parser.parse(lattice_meta_input);

            return Lattice(name, meta, std::move(lattice_data_input));
        }
    } // namespace volume_importer
} // namespace regina