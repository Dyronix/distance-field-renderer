#include "rex_graphics_pch.h"

#include "perspectivecamera.h"
#include "algortihm.h"

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
        , m_projection_matrix(rex::perspective(m_field_of_view.getVertical().to_rad(), m_aspect_ratio.getRatio().get(), clippingPlanes.near_plane,
                                               clippingPlanes.far_plane))
        , m_view_matrix(rex::identity<rex::matrix4>())
    {
    }
    //-------------------------------------------------------------------------
    PerspectiveCamera::~PerspectiveCamera() = default;

    //-------------------------------------------------------------------------
    void PerspectiveCamera::setPosition(const rex::vec3& position)
    {
        if (rex::is_identical(getPosition(), position))
        {
            return;
        }

        m_transform.setPosition(position);

        m_transform.reevaluate();
        updateMatrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::setRotation(const rex::quaternion& rotation)
    {
        if (rex::is_identical(getRotation(), rotation))
        {
            return;
        }

        m_transform.setRotation(rotation);

        m_transform.reevaluate();
        updateMatrices();
    }

    //-------------------------------------------------------------------------
    void PerspectiveCamera::setAspectRatio(const AspectRatio& ratio, AdjustFOV adjustFOV)
    {
        if (is_identical(m_aspect_ratio.getRatio().get(), ratio.getRatio().get()))
        {
            return;
        }

        m_aspect_ratio = ratio;

        if (adjustFOV)
        {
            m_field_of_view.reevaluate(ratio.getRatio());
        }

        m_projection_matrix = rex::perspective(m_field_of_view.getVertical().to_rad(), m_aspect_ratio.getRatio().get(), m_clipping_planes.near_plane,
                                               m_clipping_planes.far_plane);

        m_transform.reevaluate();
        updateMatrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::setVerticalFieldOfView(const DegAngle& angle)
    {
        if (is_identical(angle, m_field_of_view.getVertical()))
        {
            return;
        }

        m_field_of_view.setVertical(angle);
        m_projection_matrix = rex::perspective(m_field_of_view.getVertical().to_rad(), m_aspect_ratio.getRatio().get(), m_clipping_planes.near_plane,
                                               m_clipping_planes.far_plane);

        m_transform.reevaluate();
        updateMatrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::setHorizontalFieldOfView(const DegAngle& angle)
    {
        if (is_identical(angle, m_field_of_view.getHorizontal()))
        {
            return;
        }

        m_field_of_view.setHorizontal(angle);
        m_projection_matrix = rex::perspective(m_field_of_view.getVertical().to_rad(), m_aspect_ratio.getRatio().get(), m_clipping_planes.near_plane,
                                               m_clipping_planes.far_plane);

        m_transform.reevaluate();
        updateMatrices();
    }
    //-------------------------------------------------------------------------
    void PerspectiveCamera::setClippingPlanes(const ClippingPlanes& planes)
    {
        if (is_identical(getClippingPlanes().near_plane, planes.near_plane) && is_identical(getClippingPlanes().far_plane, planes.far_plane))
        {
            return;
        }

        m_clipping_planes = planes;
        m_projection_matrix = rex::perspective(m_field_of_view.getVertical().to_rad(), m_aspect_ratio.getRatio().get(), m_clipping_planes.near_plane,
                                               m_clipping_planes.far_plane);

        m_transform.reevaluate();
        updateMatrices();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::getPosition() const
    {
        return m_transform.getPosition();
    }
    //-------------------------------------------------------------------------
    const rex::quaternion& PerspectiveCamera::getRotation() const
    {
        return m_transform.getRotation();
    }

    //-------------------------------------------------------------------------
    const AspectRatio& PerspectiveCamera::getAspectRatio() const
    {
        return m_aspect_ratio;
    }
    //-------------------------------------------------------------------------
    const FieldOfView& PerspectiveCamera::getFieldOfView() const
    {
        return m_field_of_view;
    }
    //-------------------------------------------------------------------------
    const DegAngle& PerspectiveCamera::getVerticalFieldOfView() const
    {
        return m_field_of_view.getVertical();
    }
    //-------------------------------------------------------------------------
    const DegAngle& PerspectiveCamera::getHorizontalFieldOfView() const
    {
        return m_field_of_view.getHorizontal();
    }
    //-------------------------------------------------------------------------
    const ClippingPlanes& PerspectiveCamera::getClippingPlanes() const
    {
        return m_clipping_planes;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::getInverseViewProjectionMatrix() const
    {
        return m_inv_view_projection_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::getViewProjectionMatrix() const
    {
        return m_view_projection_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::getProjectionMatrix() const
    {
        return m_projection_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::getViewMatrix() const
    {
        return m_view_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::getFrustrumCorners() const
    {
        return m_frustrum_matrix;
    }
    //-------------------------------------------------------------------------
    const rex::matrix4& PerspectiveCamera::getWorldMatrix() const
    {
        return m_transform.getWorld();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::getUpDirection() const
    {
        return m_transform.getUp();
    }
    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::getRightDirection() const
    {
        return m_transform.getRight();
    }
    //-------------------------------------------------------------------------
    const rex::vec3& PerspectiveCamera::getForwardDirection() const
    {
        return m_transform.getForward();
    }

    //-------------------------------------------------------------------------
    rex::matrix4 PerspectiveCamera::calculateViewMatrix() const
    {
        return rex::lookAt(getPosition(), getPosition() + getForwardDirection(), rex::worldUp<float>());
    }
    //-------------------------------------------------------------------------
    rex::matrix4 PerspectiveCamera::calculateViewProjectionMatrix() const
    {
        return m_projection_matrix * m_view_matrix;
    }

    //-------------------------------------------------------------------------
    rex::matrix4 PerspectiveCamera::calculateFrustrumMatrix() const
    {
        FieldOfView field_of_view = getFieldOfView();
        AspectRatio aspect_ratio = getAspectRatio();

        rex::matrix4 frustum_corners = rex::identity<rex::matrix4>();

        float fov_v_half = field_of_view.getVertical().to_rad() * 0.5f;
        float tan_fov = std::tan(fov_v_half);

        rex::vec3 to_right = rex::worldRight<float>() * tan_fov * aspect_ratio.getRatio().get();
        rex::vec3 to_top = rex::worldUp<float>() * tan_fov;

        rex::vec3 top_left = (-rex::worldForward<float>() - to_right + to_top);
        rex::vec3 top_right = (-rex::worldForward<float>() + to_right + to_top);
        rex::vec3 bottom_right = (-rex::worldForward<float>() + to_right - to_top);
        rex::vec3 bottom_left = (-rex::worldForward<float>() - to_right - to_top);

        frustum_corners = rex::column(frustum_corners, 0, rex::vec4(top_left, 0.0f));
        frustum_corners = rex::column(frustum_corners, 1, rex::vec4(top_right, 0.0f));
        frustum_corners = rex::column(frustum_corners, 2, rex::vec4(bottom_right, 0.0f));
        frustum_corners = rex::column(frustum_corners, 3, rex::vec4(bottom_left, 0.0f));

        return frustum_corners;
    }

    //-------------------------------------------------------------------------
    void PerspectiveCamera::updateMatrices()
    {
        m_view_matrix = calculateViewMatrix();
        m_view_projection_matrix = calculateViewProjectionMatrix();
        m_frustrum_matrix = calculateFrustrumMatrix();

        m_inv_view_projection_matrix = rex::inverse(m_view_projection_matrix);
    }
}