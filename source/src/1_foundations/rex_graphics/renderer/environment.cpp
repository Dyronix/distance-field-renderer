#include "rex_graphics_pch.h"

#include "renderer/environment.h"

#include "renderer/point_light.h"
#include "renderer/directional_light.h"

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

    //-------------------------------------------------------------------------
    LightEnvironment::LightEnvironment() = default;
    //-------------------------------------------------------------------------
    LightEnvironment::~LightEnvironment() = default;

    //-------------------------------------------------------------------------
    LightEnvironment::operator bool() const
    {
        return point_lights.empty() == false && directional_lights.empty() == false;
    }
}