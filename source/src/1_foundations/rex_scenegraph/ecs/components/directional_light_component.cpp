#include "rex_scenegraph_pch.h"

#include "ecs/components/directional_light_component.h"

#include "algorithms/identical.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent()
            : is_active(true)
            , intensity(1.0f)
        {
        }
        //-------------------------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent(const DirectionalLightComponent& other)
            : is_active(other.is_active)
            , intensity(other.intensity)
        {
        }
        //-------------------------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent(DirectionalLightComponent&& other) noexcept
            : is_active(std::exchange(other.is_active, false))
            , intensity(std::exchange(other.intensity, 0.0f))
        {
        }
        //-------------------------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent(bool inActive, float inIntensity)
            : is_active(inActive)
            , intensity(inIntensity)
        {
        }

        //-------------------------------------------------------------------------
        rex::ecs::DirectionalLightComponent& DirectionalLightComponent::operator=(const DirectionalLightComponent& other)
        {
            this->is_active = other.is_active;
            this->intensity = other.intensity;

            return *this;
        }
        //-------------------------------------------------------------------------
        rex::ecs::DirectionalLightComponent& DirectionalLightComponent::operator=(DirectionalLightComponent&& other) noexcept
        {
            this->is_active = std::exchange(other.is_active, false);
            this->intensity = std::exchange(other.intensity, 0.0f);

            return *this;
        }

        //-------------------------------------------------------------------------
        bool DirectionalLightComponent::operator==(const DirectionalLightComponent& other) const
        {
            return is_active == other.is_active && is_identical(intensity, other.intensity);
        }
        //-------------------------------------------------------------------------
        bool DirectionalLightComponent::operator!=(const DirectionalLightComponent& other) const
        {
            return !(*this == other);
        }
    } // namespace ecs
} // namespace rex