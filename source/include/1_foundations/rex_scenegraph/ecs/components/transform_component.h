#pragma once

#include "transform.h"

namespace rex
{
    namespace ecs
    {
        struct TransformComponent
        {
            TransformComponent();
            TransformComponent(const TransformComponent& other);
            TransformComponent(const Transform& t);

            operator Transform& ();
            operator const Transform& () const;

            bool operator==(const TransformComponent& other) const;
            bool operator!=(const TransformComponent& other) const;

            Transform transform;
        };
    }
}