#include "rex_graphics_pch.h"

#include "orthographiccamera.h"

namespace rex
{
    //-------------------------------------------------------------------------
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projection_matrix(rex::ortho(left, right, bottom, top))
        , m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(0.0f)
    {
        recalculateViewMatrix();
    }

    //-------------------------------------------------------------------------
    void OrthographicCamera::setPosition(const rex::vec3& position)
    {
        m_position = position;
        recalculateViewMatrix();
    }
    //-------------------------------------------------------------------------
    void OrthographicCamera::setRotation(float rotation)
    {
        m_rotation = rotation;
        recalculateViewMatrix();
    }
    //-------------------------------------------------------------------------
    void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
    {
        m_projection_matrix = rex::ortho(left, right, bottom, top);
        recalculateViewMatrix();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& OrthographicCamera::getPosition() const
    {
        return m_position;
    }

    //-------------------------------------------------------------------------
    float OrthographicCamera::getRotation() const
    {
        return m_rotation;
    }

    const rex::matrix4& OrthographicCamera::getInverseViewProjectionMatrix() const
    {
        return m_inv_view_projection_matrix;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::getViewProjectionMatrix() const
    {
        return m_view_projection_matrix;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::getProjectionMatrix() const
    {
        return m_projection_matrix;
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& OrthographicCamera::getViewMatrix() const
    {
        return m_view_matrix;
    }

    //-------------------------------------------------------------------------
    void OrthographicCamera::recalculateViewMatrix()
    {
        rex::matrix4 translate = rex::translate(rex::matrix4(1.0f), m_position);
        rex::matrix4 rotate = rex::rotate(rex::matrix4(1.0f), m_rotation, rex::vec3(0, 0, 1));

        m_view_matrix = rex::inverse(translate * rotate);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;

        m_inv_view_projection_matrix = rex::inverse(m_view_projection_matrix);
    }
}
