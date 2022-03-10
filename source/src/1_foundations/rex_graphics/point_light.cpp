#include "rex_graphics_pch.h"

#include "renderer/pointlight.h"

namespace rex
{
    //-------------------------------------------------------------------------
    PointLight::PointLight()
        : position(0.0f)
        , intensity(1.0f)
        , min_attenuation(1.0f)
        , max_attenuation(10.0f)
    {
    }
    //-------------------------------------------------------------------------
    PointLight::PointLight(const PointLight& other)
        : position(other.position)
        , intensity(other.intensity)
        , min_attenuation(other.min_attenuation)
        , max_attenuation(other.max_attenuation)
    {
    }
    //-------------------------------------------------------------------------
    PointLight::PointLight(const rex::vec3& inPosition, float inIntensity, float inMinAttenuation, float inMaxAttenuation)
        : position(inPosition)
        , intensity(inIntensity)
        , min_attenuation(inMinAttenuation)
        , max_attenuation(inMaxAttenuation)
    {
    }

    //-------------------------------------------------------------------------
    bool PointLight::operator==(const PointLight& other) const
    {
        return rex::is_identical(position, other.position) && is_identical(intensity, other.intensity) &&
               is_identical(min_attenuation, other.min_attenuation) && is_identical(max_attenuation, other.max_attenuation);
    }
    //-------------------------------------------------------------------------
    bool PointLight::operator!=(const PointLight& other) const
    {
        return !(*this == other);
    }
}