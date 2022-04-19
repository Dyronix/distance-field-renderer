#include "regina_pch.h"

#include "lattice_meta_parser.h"

namespace regina
{
    namespace lattice_meta_parser
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
    } // namespace volume_meta_parser

    //-------------------------------------------------------------------------
    LatticeMetaParser::LatticeMetaParser() = default;
    //-------------------------------------------------------------------------
    LatticeMetaParser::~LatticeMetaParser() = default;

    //-------------------------------------------------------------------------
    bool LatticeMetaParser::on_parse_line(LatticeMeta& latticeMeta, const std::string& line, const char* currToken)
    {
        UNUSED_PARAM(line);

        // lattice cell size
        if (currToken[0] == 'l' && currToken[1] == 'c' && rex::token_parser_helpers::is_space(currToken[2]))
        {
            currToken += (char)3; // advance characters

            lattice_meta_parser::parse_voxel_grid_cell_size(&currToken, latticeMeta.lattice_grid_cell_size);

            return true;
        }

        // lattice grid size
        if (currToken[0] == 'l' && currToken[1] == 'g' && rex::token_parser_helpers::is_space(currToken[2]))
        {
            currToken += (char)3; // advance characters

            lattice_meta_parser::parse_voxel_grid_size(&currToken, latticeMeta.lattice_grid_size);

            return true;
        }

        // lattice grid bounds
        if (currToken[0] == 'b' && currToken[1] == 'b' && rex::token_parser_helpers::is_space(currToken[2]))
        {
            currToken += (char)3; // advance characters

            lattice_meta_parser::parse_voxel_grid_bounds(&currToken, latticeMeta.lattice_grid_bounds);

            return true;
        }

        return false;
    }
} // namespace regina