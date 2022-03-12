#include "rex_graphics_pch.h"

#include "orthographic_camera.h"

#include "matrix_transform.h"
#include "matrix_clip_space.h"

namespace rex
{
    //-------------------------------------------------------------------------
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projection_matrix(rex::ortho(left, right, bottom, top))
        , m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(0.0f)
    {
        recalculate_view_matrix();
    }

    //-------------------------------------------------------------------------
    void OrthographicCamera::set_position(const rex::vec3& position)
    {
        m_position = position;
        recalculate_view_matrix();
    }
    //-------------------------------------------------------------------------
    void OrthographicCamera::set_rotation(float rotation)
    {
        m_rotation = rotation;
        recalculate_view_matrix();
    }
    //-------------------------------------------------------------------------
    void OrthographicCamera::set_projection(float left, float right, float bottom, float top)
    {
        m_projection_matrix = rex::ortho(left, right, bottom, top);
        recalculate_view_matrix();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& OrthographicCamera::get_position() const
    {
        return m_position;
    }

    //-------------------------------------------------------------------------
    float OrthographicCamera::get_rotation() const
    {
        return m_rotation;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::get_inverse_view_projection_matrix() const
    {
        return m_inv_view_projection_matrix;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::get_view_projection_matrix() const
    {
        return m_view_projection_matrix;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::get_projection_matrix() const
    {
        return m_projection_matrix;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::get_view_matrix() const
    {
        return m_view_matrix;
    }

    //-------------------------------------------------------------------------
    void OrthographicCamera::recalculate_view_matrix()
    {
        rex::matrix4 translate = rex::translate(rex::matrix4(1.0f), m_position);
        rex::matrix4 rotate = rex::rotate(rex::matrix4(1.0f), m_rotation, rex::vec3(0, 0, 1));

        m_view_matrix = rex::inverse(translate * rotate);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;

        m_inv_view_projection_matrix = rex::inverse(m_view_projection_matrix);
    }
}
