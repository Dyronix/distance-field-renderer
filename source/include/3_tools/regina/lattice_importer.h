#pragma once

#include "lattice.h"

namespace regina
{
    namespace lattice_importer
    {
        Lattice import(const rex::StringID& name, const rex::StringID& latticeMetaPath, const rex::StringID& latticeDataPath);
    }
} // namespace regina