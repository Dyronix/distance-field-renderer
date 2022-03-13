#pragma once

namespace rex
{
    namespace ecs
    {
        struct DirectionalLightComponent
        {
            DirectionalLightComponent();
            DirectionalLightComponent(const DirectionalLightComponent& other);
            DirectionalLightComponent(bool inActive, float inIntensity);

            bool operator==(const DirectionalLightComponent& other) const;
            bool operator!=(const DirectionalLightComponent& other) const;

            bool is_active;

            float intensity;
        };
    }
}