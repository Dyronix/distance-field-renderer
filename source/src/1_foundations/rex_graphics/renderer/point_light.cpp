#include "rex_graphics_pch.h"

#include "renderer/point_light.h"

#include "algorithms/identical.h"

namespace rex
{
    //-------------------------------------------------------------------------
    PointLight::PointLight()
        : position(0.0f)
        , intensity(1.0f)
        , min_attenuation(1.0f)
        , max_attenuation(10.0f)
        , color(rex::to_color3(colors::white))
    {
    }
    //-------------------------------------------------------------------------
    PointLight::PointLight(const PointLight& other)
        : position(other.position)
        , intensity(other.intensity)
        , min_attenuation(other.min_attenuation)
        , max_attenuation(other.max_attenuation)
        , color(other.color)
    {
    }
    //-------------------------------------------------------------------------
    PointLight::PointLight(const rex::vec3& inPosition, float inIntensity, float inMinAttenuation, float inMaxAttenuation, const rex::ColorRGB& inColor)
        : position(inPosition)
        , intensity(inIntensity)
        , min_attenuation(inMinAttenuation)
        , max_attenuation(inMaxAttenuation)
        , color(inColor)
    {
    }

    //-------------------------------------------------------------------------
    bool PointLight::operator==(const PointLight& other) const
    {
        return rex::is_identical(position, other.position) && is_identical(intensity, other.intensity) && is_identical(min_attenuation, other.min_attenuation) && is_identical(max_attenuation, other.max_attenuation) && color == other.color;
    }
    //-------------------------------------------------------------------------
    bool PointLight::operator!=(const PointLight& other) const
    {
        return !(*this == other);
    }
}