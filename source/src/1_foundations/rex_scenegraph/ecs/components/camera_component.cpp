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
        CameraComponent::CameraComponent(const SceneCamera& t, bool hasFixedAspectRatio)
            : camera(t)
            , has_fixed_aspect_ratio(hasFixedAspectRatio)
        {
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
    }
}