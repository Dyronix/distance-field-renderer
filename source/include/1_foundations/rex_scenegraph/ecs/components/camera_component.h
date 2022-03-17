#pragma once

#include "ecs/scene_camera.h"

namespace rex
{
    namespace ecs
    {
        struct CameraComponent
        {
            CameraComponent();
            CameraComponent(const CameraComponent& other);
            CameraComponent(CameraComponent&& other) noexcept;
            CameraComponent(const SceneCamera& t, bool hasFixedAspectRatio = false);

            CameraComponent& operator=(const CameraComponent& other);
            CameraComponent& operator=(CameraComponent&& other) noexcept;

            operator SceneCamera&();
            operator const SceneCamera&() const;

            bool operator==(const CameraComponent& other) const;
            bool operator!=(const CameraComponent& other) const;

            SceneCamera camera;

            bool has_fixed_aspect_ratio;
        };
    }
}