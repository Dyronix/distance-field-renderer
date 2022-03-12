#pragma once

#include "resources/texture_cube.h"

namespace rex
{
    struct Environment
    {
        Environment();
        ~Environment();

        operator bool() const;

        ref_ptr<TextureCube> skybox_texture_cube;
    };
}