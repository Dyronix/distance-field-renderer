#pragma once

#include "resources/texture_cube.h"

namespace rex
{
    struct PointLight;
    struct DirectionalLight;

    struct Environment
    {
        Environment();
        ~Environment();

        operator bool() const;

        ref_ptr<TextureCube> skybox_texture_cube;
    };

    struct LightEnvironment
    {
        LightEnvironment();
        ~LightEnvironment();

        operator bool() const;

        std::unordered_map<int32, PointLight> point_lights;
        std::unordered_map<int32, DirectionalLight> directional_lights;
    };
}