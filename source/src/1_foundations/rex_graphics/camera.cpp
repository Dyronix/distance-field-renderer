#include "rex_graphics_pch.h"

#include "camera.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Camera::Camera()
        : m_projection(rex::matrix4(1.0f))
        , m_aspect_ratio(1.0f, 1.0f)
    {
    }
    //-------------------------------------------------------------------------
    Camera::Camera(const AspectRatio& aspectRatio, const rex::matrix4& projection)
        : m_projection(projection)
        , m_aspect_ratio(aspectRatio)
    {
    }

    //-------------------------------------------------------------------------
    Camera::~Camera()
    {
    }

    //-------------------------------------------------------------------------
    void Camera::set_viewport_size(int32 width, int32 height)
    {
        R_ASSERT(width > 0 && height > 0);

        m_aspect_ratio = AspectRatio((float)width, (float)height);

        on_set_viewport_size(width, height);
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& Camera::get_projection_matrix() const
    {
        return m_projection;
    }

    //-------------------------------------------------------------------------
    const rex::AspectRatio Camera::get_aspect_ratio() const
    {
        return m_aspect_ratio;
    }

    //-------------------------------------------------------------------------
    void Camera::set_projection_matrix(const rex::matrix4& projection)
    {
        m_projection = projection;
    }

    //-------------------------------------------------------------------------
    bool Camera::operator==(const Camera& other) const
    {
        return m_projection == other.m_projection;
    }

    //-------------------------------------------------------------------------
    bool Camera::operator!=(const Camera& other) const
    {
        return !(*this == other);
    }
}