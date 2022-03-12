#include "rex_graphics_pch.h"

#include "renderer/environment.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Environment::Environment() = default;
    //-------------------------------------------------------------------------
    Environment::~Environment() = default;

    //-------------------------------------------------------------------------
    Environment::operator bool() const
    {
        return skybox_texture_cube != nullptr;
    }
}