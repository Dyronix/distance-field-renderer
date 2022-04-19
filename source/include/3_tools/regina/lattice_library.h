#pragma once

namespace rex
{
    class Texture;
}

namespace regina
{
    struct LatticeMeta;

    class Lattice;

    namespace lattice_library
    {
        bool add(Lattice&& lattice);

        Lattice& get_lattice(const rex::StringID& latticeName);
        rex::ref_ptr<rex::Texture>& get_lattice_data(const rex::StringID& latticeName);

        void clear();
    }
}