#pragma once

#include "camera.h"

#include "field_of_view.h"
#include "clipping_planes.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera : public Camera
        {
        public:
            using OrthographicSize = float;

            enum class ProjectionType 
            {
                PERSPECTIVE     = 0,
                ORTHOGRAPHIC    = 1 
            };

            SceneCamera();
            ~SceneCamera() override;

            void activate();
            void deactivate();

            bool is_active() const;

            void set_projection_type(const ProjectionType& type);

            const ProjectionType& get_projection_type() const;

            void set_perspective(const FieldOfView& fov, const ClippingPlanes& clip);
            void set_perspective_vertical_fov(const DegAngle& angle);
            void set_perspective_near_plane(float nearPlane);
            void set_perspective_far_plane(float farPlane);

            void set_orthographic(const OrthographicSize& size, const ClippingPlanes& clip);
            void set_orthographic_size(const OrthographicSize& size);
            void set_orthographic_near_plane(float nearPlane);
            void set_orthographic_far_plane(float farPlane);

            const FieldOfView& get_perspective_field_of_view() const;
            float get_perspective_near_plane() const;
            float get_perspective_far_plane() const;

            const OrthographicSize& get_orthographic_size() const;
            float get_orthographic_near_plane() const;
            float get_orthographic_far_plane() const;

        protected:
            void recalculate_projection();

            void on_set_viewport_size(int32 width, int32 height) override;

        private:

            ProjectionType m_projection_type;

            FieldOfView m_perspective_fov;
            ClippingPlanes m_perspective_clip;

            OrthographicSize m_ortho_size;
            ClippingPlanes m_ortho_clip;

            bool m_active;
        };
    }
}