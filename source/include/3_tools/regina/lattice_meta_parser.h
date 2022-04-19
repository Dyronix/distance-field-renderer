#pragma once

#include "token_parser.h"
#include "lattice_meta.h"

namespace regina
{
    class LatticeMetaParser : public rex::TokenParser<LatticeMeta>
    {
    public:
        LatticeMetaParser();
        ~LatticeMetaParser() override;

    protected:
        bool on_parse_line(LatticeMeta& latticeMeta, const std::string& line, const char* currToken) override;
    };
} // namespace regina