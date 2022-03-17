#pragma once

#include "color.h"

namespace rex
{
    struct PointLight
    {
        PointLight();
        PointLight(const PointLight& other);
        PointLight(const rex::vec3& inPosition, float inIntensity, float inConstant, float inLinear, float inQuadratic, const rex::ColorRGB& inColor);

        bool operator==(const PointLight& other) const;
        bool operator!=(const PointLight& other) const;

        rex::vec3 position;
        rex::ColorRGB color;

        float intensity;

        float constant;
        float linear;
        float quadratic;
    };
}