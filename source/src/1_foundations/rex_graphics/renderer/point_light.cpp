#include "rex_graphics_pch.h"

#include "renderer/point_light.h"

#include "algorithms/identical.h"

namespace rex
{
    //-------------------------------------------------------------------------
    PointLight::PointLight()
        : position(0.0f)
        , intensity(1.0f)
        , constant(1.0f)
        , linear(0.7f)
        , quadratic(1.8f)
        , color(rex::to_color3(colors::white))
    {
    }
    //-------------------------------------------------------------------------
    PointLight::PointLight(const PointLight& other)
        : position(other.position)
        , intensity(other.intensity)
        , constant(other.constant)
        , linear(other.linear)
        , quadratic(other.quadratic)
        , color(other.color)
    {
    }
    //-------------------------------------------------------------------------
    PointLight::PointLight(const rex::vec3& inPosition, float inIntensity, float inConstant, float inLinear, float inQuadratic, const ColorRGB& inColor)
        : position(inPosition)
        , intensity(inIntensity)
        , constant(inConstant)
        , linear(inLinear)
        , quadratic(inQuadratic)
        , color(inColor)
    {
    }

    //-------------------------------------------------------------------------
    bool PointLight::operator==(const PointLight& other) const
    {
        return is_identical(intensity, other.intensity) && is_identical(constant, other.constant) && is_identical(linear, other.linear) && is_identical(quadratic, other.quadratic) && color == other.color;
    }
    //-------------------------------------------------------------------------
    bool PointLight::operator!=(const PointLight& other) const
    {
        return !(*this == other);
    }
}