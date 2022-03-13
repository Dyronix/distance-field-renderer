#pragma once

namespace rex
{
    namespace ecs
    {
        struct PointLightComponent
        {
            PointLightComponent();
            PointLightComponent(const PointLightComponent& other);
            PointLightComponent(float inIntensity, float inMinAttenuation, float inMaxAttenuation);

            bool operator==(const PointLightComponent& other) const;
            bool operator!=(const PointLightComponent& other) const;

            float intensity;

            float min_attenuation;
            float max_attenuation;
        };
    }
}