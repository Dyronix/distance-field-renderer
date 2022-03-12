#pragma once

#include "resources/texture_cube.h"

namespace rex
{   
    struct Environment
    {
        //-------------------------------------------------------------------------
        operator bool() const
        {
            return skybox_texture_cube != nullptr;
        }

        ref_ptr<TextureCube> skybox_texture_cube;
    };
}