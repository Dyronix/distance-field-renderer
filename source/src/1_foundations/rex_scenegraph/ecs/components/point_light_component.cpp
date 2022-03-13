#include "rex_scenegraph_pch.h"

#include "ecs/components/point_light_component.h"

#include "algorithms/identical.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        PointLightComponent::PointLightComponent()
            : intensity(1.0f)
            , min_attenuation(1.0f)
            , max_attenuation(10.0f)
        {}
        //-------------------------------------------------------------------------
        PointLightComponent::PointLightComponent(const PointLightComponent& other)
            : intensity(other.intensity)
            , min_attenuation(other.min_attenuation)
            , max_attenuation(other.max_attenuation)
        {}
        //-------------------------------------------------------------------------
        PointLightComponent::PointLightComponent(float inIntensity, float inMinAttenuation, float inMaxAttenuation)
            : intensity(inIntensity)
            , min_attenuation(inMinAttenuation)
            , max_attenuation(inMaxAttenuation)
        {}

        //-------------------------------------------------------------------------
        bool PointLightComponent::operator==(const PointLightComponent& other) const
        {
            return is_identical(intensity, other.intensity)
                && is_identical(min_attenuation, other.min_attenuation)
                && is_identical(max_attenuation, other.max_attenuation);
        }
        //-------------------------------------------------------------------------
        bool PointLightComponent::operator!=(const PointLightComponent& other) const
        {
            return !(*this == other);
        }

    }
}