#include "rex_graphics_pch.h"

#include "perspective_camera.h"

#include "common.h"
#include "matrix_transform.h"
#include "matrix_clip_space.h"

#include "algorithms/identical.h"

namespace rex
{
    //-------------------------------------------------------------------------
    PerspectiveCamera::PerspectiveCamera(const FieldOfView& fieldOfView, const AspectRatio& aspectRatio, const ClippingPlanes& clippingPlanes)
        : m_transform(rex::vec3(0.0f, 2.0f, 4.0f))
        , m_aspect_ratio(aspectRatio)
        , m_field_of_view(fieldOfView)
        , m_clipping_planes(clippingPlanes)
        , m_inv_view_projection_matrix(rex::identity<rex::matrix4>())
        , m_view_projection_matrix(rex::identity<rex::matrix4>())
        , m_projection_matrix(rex::perspective(m_field_of_view.get_vertical().to_rad(), m_aspect_ratio.get_ratio().get(), clippingPlanes.near_plane, clippingPlanes.far_plane))
        , m_view_matrix(rex::identity<rex::matrix4>())
    {
    }
    //-------------------------------------------------------------------------
    PerspectiveCamera::~PerspectiveCamera() = default;

    //-------------------------------------------------------------------------
    void PerspectiveCamera::set_position(const rex::vec3& position)
    {
        if (rex::is_identical(get_position(), position))
        {
            return;
        }

        m_transform.set_position(position);

        m_transform.reevaluate();
        update_matrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::set_rotation(const rex::quaternion& rotation)
    {
        if (rex::is_identical(get_rotation(), rotation))
        {
            return;
        }

        m_transform.set_rotation(rotation);

        m_transform.reevaluate();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void PerspectiveCamera::set_aspect_ratio(const AspectRatio& ratio, AdjustFOV adjustFOV)
    {
        if (is_identical(m_aspect_ratio.get_ratio().get(), ratio.get_ratio().get()))
        {
            return;
        }

        m_aspect_ratio = ratio;

        if (adjustFOV)
        {
            m_field_of_view.reevaluate(ratio.get_ratio());
        }

        m_projection_matrix = rex::perspective(m_field_of_view.get_vertical().to_rad(), m_aspect_ratio.get_ratio().get(), m_clipping_planes.near_plane, m_clipping_planes.far_plane);

        m_transform.reevaluate();
        update_matrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::set_vertical_field_of_view(const DegAngle& angle)
    {
        if (is_identical(angle, m_field_of_view.get_vertical()))
        {
            return;
        }

        m_field_of_view.set_vertical(angle);
        m_projection_matrix = rex::perspective(m_field_of_view.get_vertical().to_rad(), m_aspect_ratio.get_ratio().get(), m_clipping_planes.near_plane, m_clipping_planes.far_plane);

        m_transform.reevaluate();
        update_matrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::set_horizontal_field_of_view(const DegAngle& angle)
    {
        if (is_identical(angle, m_field_of_view.get_horizontal()))
        {
            return;
        }

        m_field_of_view.set_horizontal(angle);
        m_projection_matrix = rex::perspective(m_field_of_view.get_vertical().to_rad(), m_aspect_ratio.get_ratio().get(), m_clipping_planes.near_plane, m_clipping_planes.far_plane);

        m_transform.reevaluate();
        update_matrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::set_clipping_planes(const ClippingPlanes& planes)
    {
        if (is_identical(get_clipping_planes().near_plane, planes.near_plane) && is_identical(get_clipping_planes().far_plane, planes.far_plane))
        {
            return;
        }

        m_clipping_planes = planes;
        m_projection_matrix = rex::perspective(m_field_of_view.get_vertical().to_rad(), m_aspect_ratio.get_ratio().get(), m_clipping_planes.near_plane, m_clipping_planes.far_plane);

        m_transform.reevaluate();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::get_position() const
    {
        return m_transform.get_position();
    }
    //-------------------------------------------------------------------------
    const rex::quaternion& PerspectiveCamera::get_rotation() const
    {
        return m_transform.get_rotation();
    }

    //-------------------------------------------------------------------------
    const AspectRatio& PerspectiveCamera::get_aspect_ratio() const
    {
        return m_aspect_ratio;
    }
    //-------------------------------------------------------------------------
    const FieldOfView& PerspectiveCamera::get_field_of_view() const
    {
        return m_field_of_view;
    }
    //-------------------------------------------------------------------------
    const DegAngle& PerspectiveCamera::get_vertical_field_of_view() const
    {
        return m_field_of_view.get_vertical();
    }
    //-------------------------------------------------------------------------
    const DegAngle& PerspectiveCamera::get_horizontal_field_of_view() const
    {
        return m_field_of_view.get_horizontal();
    }
    //-------------------------------------------------------------------------
    const ClippingPlanes& PerspectiveCamera::get_clipping_planes() const
    {
        return m_clipping_planes;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::get_inverse_view_projection_matrix() const
    {
        return m_inv_view_projection_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::get_view_projection_matrix() const
    {
        return m_view_projection_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::get_projection_matrix() const
    {
        return m_projection_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::get_view_matrix() const
    {
        return m_view_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::get_frustrum_corners() const
    {
        return m_frustrum_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::get_world_matrix() const
    {
        return m_transform.get_world();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::get_up_direction() const
    {
        return m_transform.get_up();
    }
    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::get_right_direction() const
    {
        return m_transform.get_right();
    }
    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::get_forward_direction() const
    {
        return m_transform.get_forward();
    }

    //-------------------------------------------------------------------------
    rex::matrix4 PerspectiveCamera::calculate_view_matrix() const
    {
        return rex::look_at(get_position(), get_position() + get_forward_direction(), rex::world_up<float>());
    }
    //-------------------------------------------------------------------------
    rex::matrix4 PerspectiveCamera::calculate_view_projection_matrix() const
    {
        return m_projection_matrix * m_view_matrix;
    }

    //-------------------------------------------------------------------------
    rex::matrix4 PerspectiveCamera::calculate_frustrum_matrix() const
    {
        FieldOfView field_of_view = get_field_of_view();
        AspectRatio aspect_ratio = get_aspect_ratio();

        rex::matrix4 frustum_corners = rex::identity<rex::matrix4>();

        float fov_v_half = field_of_view.get_vertical().to_rad() * 0.5f;
        float tan_fov = std::tan(fov_v_half);

        rex::vec3 to_right = rex::world_right<float>() * tan_fov * aspect_ratio.get_ratio().get();
        rex::vec3 to_top = rex::world_up<float>() * tan_fov;

        rex::vec3 top_left = (-rex::world_forward<float>() - to_right + to_top);
        rex::vec3 top_right = (-rex::world_forward<float>() + to_right + to_top);
        rex::vec3 bottom_right = (-rex::world_forward<float>() + to_right - to_top);
        rex::vec3 bottom_left = (-rex::world_forward<float>() - to_right - to_top);

        frustum_corners = rex::column(frustum_corners, 0, rex::vec4(top_left, 0.0f));
        frustum_corners = rex::column(frustum_corners, 1, rex::vec4(top_right, 0.0f));
        frustum_corners = rex::column(frustum_corners, 2, rex::vec4(bottom_right, 0.0f));
        frustum_corners = rex::column(frustum_corners, 3, rex::vec4(bottom_left, 0.0f));

        return frustum_corners;
    }

    //-------------------------------------------------------------------------
    void PerspectiveCamera::update_matrices()
    {
        m_view_matrix = calculate_view_matrix();
        m_view_projection_matrix = calculate_view_projection_matrix();
        m_frustrum_matrix = calculate_frustrum_matrix();

        m_inv_view_projection_matrix = rex::inverse(m_view_projection_matrix);
    }
}