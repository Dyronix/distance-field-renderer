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
            TransformComponent(TransformComponent&& other) noexcept;
            TransformComponent(const Transform& t);

            TransformComponent& operator=(const TransformComponent& other);
            TransformComponent& operator=(TransformComponent&& other) noexcept;

            operator Transform&();
            operator const Transform&() const;

            bool operator==(const TransformComponent& other) const;
            bool operator!=(const TransformComponent& other) const;

            Transform transform;
        };
    }
}