#pragma once

#include "token_parser.h"

namespace regina
{  
    class VolumeMetaParser : public rex::TokenParser<VolumeMeta>
    {
    public:
        VolumeMetaParser();
        ~VolumeMetaParser() override;

    protected:
        bool on_parse_line(VolumeMeta& volumeMeta, const std::string& line, const char* currToken) override;
    };
}