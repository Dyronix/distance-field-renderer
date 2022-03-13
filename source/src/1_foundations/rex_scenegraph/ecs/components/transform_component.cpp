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
    }
}