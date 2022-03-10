#include "rex_graphics_pch.h"

#include "renderer/directionallight.h"

namespace rex
{
    //-------------------------------------------------------------------------
    DirectionalLight::DirectionalLight()
        : direction(1.0f)
        , intensity(1.0f)
    {
    }
    //-------------------------------------------------------------------------
    DirectionalLight::DirectionalLight(const DirectionalLight& other)
        : direction(other.direction)
        , intensity(other.intensity)
    {
    }
    //-------------------------------------------------------------------------
    DirectionalLight::DirectionalLight(const rex::vec3& inDirection, float inIntensity)
        : direction(inDirection)
        , intensity(inIntensity)
    {
    }

    //-------------------------------------------------------------------------
    bool DirectionalLight::operator==(const DirectionalLight& other) const
    {
        return rex::is_identical(direction, other.direction) && is_identical(intensity, other.intensity);
    }
    //-------------------------------------------------------------------------
    bool DirectionalLight::operator!=(const DirectionalLight& other) const
    {
        return !(*this == other);
    }
}