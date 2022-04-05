#include "regina_pch.h"

#include "volume_meta_parser.h"

namespace regina
{
    namespace volume_meta_parser
    {
        //-------------------------------------------------------------------------
        void parse_voxel_grid_cell_size(const char** token, rex::vec3& cellSize)
        {
            cellSize = rex::vec3(rex::token_parser_helpers::parse_float(token));
        }
        //-------------------------------------------------------------------------
        void parse_voxel_grid_size(const char** token, rex::vec3& gridSize)
        {
            gridSize.x = rex::token_parser_helpers::parse_float(token);
            gridSize.y = rex::token_parser_helpers::parse_float(token);
            gridSize.z = rex::token_parser_helpers::parse_float(token);
        }
        //-------------------------------------------------------------------------
        void parse_voxel_grid_bounds(const char** token, rex::MinMax<rex::vec3>& bounds)
        {
            bounds.minimum.x = rex::token_parser_helpers::parse_float(token);
            bounds.minimum.y = rex::token_parser_helpers::parse_float(token);
            bounds.minimum.z = rex::token_parser_helpers::parse_float(token);

            bounds.maximum.x = rex::token_parser_helpers::parse_float(token);
            bounds.maximum.y = rex::token_parser_helpers::parse_float(token);
            bounds.maximum.z = rex::token_parser_helpers::parse_float(token);
        }
    }

    //-------------------------------------------------------------------------
    VolumeMetaParser::VolumeMetaParser() = default;
    //-------------------------------------------------------------------------
    VolumeMetaParser::~VolumeMetaParser() = default;

    //-------------------------------------------------------------------------
    bool VolumeMetaParser::on_parse_line(VolumeMeta& volumeMeta, const std::string& line, const char* currToken)
    {
        UNUSED_PARAM(line);

        // voxel cell size
        if (currToken[0] == 'v' && currToken[1] == 'c' && rex::token_parser_helpers::is_space(currToken[2]))
        {
            currToken += (char)3; // advance characters

            volume_meta_parser::parse_voxel_grid_cell_size(&currToken, volumeMeta.voxel_grid_cell_size);

            return true;
        }

        // voxel grid size
        if (currToken[0] == 'v' && currToken[1] == 'g' && rex::token_parser_helpers::is_space(currToken[2]))
        {
            currToken += (char)3; // advance characters

            volume_meta_parser::parse_voxel_grid_size(&currToken, volumeMeta.voxel_grid_size);

            return true;
        }

        // voxel grid bounds
        if (currToken[0] == 'b' && currToken[1] == 'b' && rex::token_parser_helpers::is_space(currToken[2]))
        {
            currToken += (char)3; // advance characters

            volume_meta_parser::parse_voxel_grid_bounds(&currToken, volumeMeta.voxel_grid_bounds);

            return true;
        }

        return false;
    }
}