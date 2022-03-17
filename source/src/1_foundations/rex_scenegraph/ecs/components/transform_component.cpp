#include "rex_scenegraph_pch.h"

#include "ecs/components/transform_component.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        TransformComponent::TransformComponent()
            : transform()
        {
        }
        //-------------------------------------------------------------------------
        TransformComponent::TransformComponent(const TransformComponent& other)
            : transform(other.transform)
        {
        }
        //-------------------------------------------------------------------------
        TransformComponent::TransformComponent(const Transform& t)
            : transform(t)
        {
        }
        //-------------------------------------------------------------------------
        TransformComponent::TransformComponent(TransformComponent&& other) noexcept
            : transform(std::exchange(other.transform, {}))
        {
        }

        //-------------------------------------------------------------------------
        rex::ecs::TransformComponent& TransformComponent::operator=(const TransformComponent& other)
        {
            this->transform = other.transform;

            return *this;
        }

        //-------------------------------------------------------------------------
        rex::ecs::TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
        {
            this->transform = std::exchange(other.transform, {});
            return *this;
        }

        //-------------------------------------------------------------------------
        TransformComponent::operator Transform&()
        {
            return transform;
        }
        //-------------------------------------------------------------------------
        TransformComponent::operator const Transform&() const
        {
            return transform;
        }

        //-------------------------------------------------------------------------
        bool TransformComponent::operator==(const TransformComponent& other) const
        {
            return transform == other.transform;
        }
        //-------------------------------------------------------------------------
        bool TransformComponent::operator!=(const TransformComponent& other) const
        {
            return !(*this == other);
        }
    } // namespace ecs
} // namespace rex