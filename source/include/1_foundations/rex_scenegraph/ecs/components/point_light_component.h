#pragma once

#include "color.h"

namespace rex
{
    namespace ecs
    {
        struct PointLightComponent
        {
            PointLightComponent();
            PointLightComponent(const PointLightComponent& other);
            PointLightComponent(PointLightComponent&& other) noexcept;
            PointLightComponent(float inIntensity, float inConstant, float inLinear, float inQuadratic, const ColorRGB& inColor);

            PointLightComponent& operator=(const PointLightComponent& other);
            PointLightComponent& operator=(PointLightComponent&& other) noexcept;

            bool operator==(const PointLightComponent& other) const;
            bool operator!=(const PointLightComponent& other) const;

            float intensity;

            float constant;
            float linear;
            float quadratic;

            ColorRGB color;
        };
    }
}