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
            , constant(1.0f)
            , linear(0.7f)
            , quadratic(1.8f)
            , color(ColorRGB(1.0f, 1.0f, 1.0))
        {
        }
        //-------------------------------------------------------------------------
        PointLightComponent::PointLightComponent(const PointLightComponent& other)
            : intensity(other.intensity)
            , constant(other.constant)
            , linear(other.linear)
            , quadratic(other.quadratic)
            , color(other.color)
        {
        }
        //-------------------------------------------------------------------------
        PointLightComponent::PointLightComponent(PointLightComponent&& other) noexcept
            : intensity(std::exchange(other.intensity, 0.0f))
            , constant(std::exchange(other.constant, {}))
            , linear(std::exchange(other.linear, {}))
            , quadratic(std::exchange(other.quadratic, {}))
            , color(std::exchange(other.color, {}))
        {
        }
        //-------------------------------------------------------------------------
        PointLightComponent::PointLightComponent(float inIntensity, float inConstant, float inLinear, float inQuadratic, const ColorRGB& inColor)
            : intensity(inIntensity)
            , constant(inConstant)
            , linear(inLinear)
            , quadratic(inQuadratic)
            , color(inColor)
        {
        }

        //-------------------------------------------------------------------------
        rex::ecs::PointLightComponent& PointLightComponent::operator=(const PointLightComponent& other)
        {
            this->intensity = other.intensity;
            this->constant = other.constant;
            this->linear = other.linear;
            this->quadratic = other.quadratic;
            this->color = other.color;

            return *this;
        }
        //-------------------------------------------------------------------------
        rex::ecs::PointLightComponent& PointLightComponent::operator=(PointLightComponent&& other) noexcept
        {
            this->intensity = std::exchange(other.intensity, 0.0f);
            this->constant = std::exchange(other.constant, 0.0f);
            this->linear = std::exchange(other.linear, 0.0f);
            this->quadratic = std::exchange(other.quadratic, 0.0f);
            this->color = std::exchange(other.color, {});

            return *this;
        }

        //-------------------------------------------------------------------------
        bool PointLightComponent::operator==(const PointLightComponent& other) const
        {
            return is_identical(intensity, other.intensity) 
                && is_identical(constant, other.constant) 
                && is_identical(linear, other.linear) 
                && is_identical(quadratic, other.quadratic) 
                && color == other.color;
        }
        //-------------------------------------------------------------------------
        bool PointLightComponent::operator!=(const PointLightComponent& other) const
        {
            return !(*this == other);
        }
    } // namespace ecs
} // namespace rex