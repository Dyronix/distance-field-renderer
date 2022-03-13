#include "rex_scenegraph_pch.h"

#include "ecs/scene_camera.h"

namespace rex
{
    namespace ecs
    {
        static AspectRatio DEFAULT_ASPECT = AspectRatio(16.0f, 9.0f);

        //-------------------------------------------------------------------------
        SceneCamera::SceneCamera()
            :Camera(DEFAULT_ASPECT)
            ,m_projection_type(ProjectionType::PERSPECTIVE)
            ,m_perspective_fov(45_deg, DEFAULT_ASPECT.get_ratio())
            ,m_perspective_clip(0.01f, 1000.0f)
            ,m_ortho_size(10.0f)
            ,m_ortho_clip(0.01f, 1000.0f)
            ,m_active(false)
        {
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        SceneCamera::~SceneCamera()
        {

        }

        //-------------------------------------------------------------------------
        void SceneCamera::activate()
        {
            m_active = true;
        }

        //-------------------------------------------------------------------------
        void SceneCamera::deactivate()
        {
            m_active = false;
        }

        //-------------------------------------------------------------------------
        bool SceneCamera::is_active() const
        {
            return m_active;
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_projection_type(const ProjectionType& type)
        {
            m_projection_type = type;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        const rex::ecs::SceneCamera::ProjectionType& SceneCamera::get_projection_type() const
        {
            return m_projection_type;
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_perspective(const FieldOfView& fov, const ClippingPlanes& clip)
        {
            m_perspective_fov = fov;
            m_perspective_clip = clip;
            m_projection_type = ProjectionType::PERSPECTIVE;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_perspective_vertical_fov(const DegAngle& angle)
        {
            m_perspective_fov = FieldOfView(angle, get_aspect_ratio());
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_perspective_near_plane(float nearPlane)
        {
            m_perspective_clip.near_plane = nearPlane;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_perspective_far_plane(float farPlane)
        {
            m_perspective_clip.far_plane = farPlane;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_orthographic(const OrthographicSize& size, const ClippingPlanes& clip)
        {
            m_ortho_size = size;
            m_ortho_clip = clip;
            m_projection_type = ProjectionType::ORTHOGRAPHIC;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_orthographic_size(const OrthographicSize& size)
        {
            m_ortho_size = size;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_orthographic_near_plane(float nearPlane)
        {
            m_ortho_clip.near_plane = nearPlane;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::set_orthographic_far_plane(float farPlane)
        {
            m_ortho_clip.far_plane = farPlane;
            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        const rex::FieldOfView& SceneCamera::get_perspective_field_of_view() const
        {
            return m_perspective_fov;
        }

        //-------------------------------------------------------------------------
        float SceneCamera::get_perspective_near_plane() const
        {
            return m_perspective_clip.near_plane;
        }

        //-------------------------------------------------------------------------
        float SceneCamera::get_perspective_far_plane() const
        {
            return m_perspective_clip.far_plane;
        }

        //-------------------------------------------------------------------------
        const rex::ecs::SceneCamera::OrthographicSize& SceneCamera::get_orthographic_size() const
        {
            return m_ortho_size;
        }

        //-------------------------------------------------------------------------
        float SceneCamera::get_orthographic_near_plane() const
        {
            return m_ortho_clip.near_plane;
        }

        //-------------------------------------------------------------------------
        float SceneCamera::get_orthographic_far_plane() const
        {
            return m_ortho_clip.far_plane;
        }

        //-------------------------------------------------------------------------
        void SceneCamera::on_set_viewport_size(int32 width, int32 height)
        {
            UNUSED_PARAM(width);
            UNUSED_PARAM(height);

            recalculate_projection();
        }

        //-------------------------------------------------------------------------
        void SceneCamera::recalculate_projection()
        {
            if (m_projection_type == ProjectionType::PERSPECTIVE)
            {
                float aspect = get_aspect_ratio().get();
                float near_plane = m_perspective_clip.near_plane;
                float far_plane = m_perspective_clip.far_plane;

                float fov = m_perspective_fov.get_vertical().get();

                set_projection_matrix(rex::perspective(fov, aspect, near_plane, far_plane));
            }
            else
            {
                float aspect = get_aspect_ratio().get();
                float near_plane = m_ortho_clip.near_plane;
                float far_plane = m_ortho_clip.far_plane;

                float ortho_left = -m_ortho_size * aspect * 0.5f;
                float ortho_right = m_ortho_size * aspect * 0.5f;
                float ortho_bottom = -m_ortho_size * 0.5f;
                float ortho_top = m_ortho_size * 0.5f;

                set_projection_matrix(rex::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, near_plane, far_plane));
            }
        }
    }
}