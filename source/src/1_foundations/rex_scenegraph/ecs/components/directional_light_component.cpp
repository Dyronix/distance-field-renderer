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
        DirectionalLightComponent::DirectionalLightComponent(bool inActive, float inIntensity)
            : is_active(inActive)
            , intensity(inIntensity)
        {
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
    }
}