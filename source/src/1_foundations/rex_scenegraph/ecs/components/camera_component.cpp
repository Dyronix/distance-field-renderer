#include "rex_scenegraph_pch.h"

#include "ecs/components/camera_component.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        CameraComponent::CameraComponent()
            : camera()
            , has_fixed_aspect_ratio(false)
        {
        }
        //-------------------------------------------------------------------------
        CameraComponent::CameraComponent(const CameraComponent& other)
            : camera(other.camera)
            , has_fixed_aspect_ratio(other.has_fixed_aspect_ratio)
        {
        }
        //-------------------------------------------------------------------------
        CameraComponent::CameraComponent(CameraComponent&& other) noexcept
            : camera(std::exchange(other.camera, {}))
            , has_fixed_aspect_ratio(std::exchange(other.has_fixed_aspect_ratio, {}))
        {
        }
        //-------------------------------------------------------------------------
        CameraComponent::CameraComponent(const SceneCamera& t, bool hasFixedAspectRatio)
            : camera(t)
            , has_fixed_aspect_ratio(hasFixedAspectRatio)
        {
        }

        //-------------------------------------------------------------------------
        rex::ecs::CameraComponent& CameraComponent::operator=(const CameraComponent& other)
        {
            this->camera = other.camera;
            this->has_fixed_aspect_ratio = other.has_fixed_aspect_ratio;

            return *this;
        }

        //-------------------------------------------------------------------------
        rex::ecs::CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept
        {
            this->camera = std::exchange(other.camera, {});
            this->has_fixed_aspect_ratio = std::exchange(other.has_fixed_aspect_ratio, false);

            return *this;
        }

        //-------------------------------------------------------------------------
        CameraComponent::operator SceneCamera&()
        {
            return camera;
        }
        //-------------------------------------------------------------------------
        CameraComponent::operator const SceneCamera&() const
        {
            return camera;
        }

        //-------------------------------------------------------------------------
        bool CameraComponent::operator==(const CameraComponent& other) const
        {
            return camera == other.camera && has_fixed_aspect_ratio == other.has_fixed_aspect_ratio;
        }
        //-------------------------------------------------------------------------
        bool CameraComponent::operator!=(const CameraComponent& other) const
        {
            return !(*this == other);
        }
    } // namespace ecs
} // namespace rex