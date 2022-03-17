#pragma once

namespace rex
{
    namespace ecs
    {
        struct DirectionalLightComponent
        {
            DirectionalLightComponent();
            DirectionalLightComponent(const DirectionalLightComponent& other);
            DirectionalLightComponent(DirectionalLightComponent&& other) noexcept;
            DirectionalLightComponent(bool inActive, float inIntensity);

            DirectionalLightComponent& operator=(const DirectionalLightComponent& other);
            DirectionalLightComponent& operator=(DirectionalLightComponent&& other) noexcept;

            bool operator==(const DirectionalLightComponent& other) const;
            bool operator!=(const DirectionalLightComponent& other) const;

            bool is_active;

            float intensity;
        };
    }
}