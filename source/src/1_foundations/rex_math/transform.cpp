#include "rex_math_pch.h"

#include "transform.h"

#include "common.h"
#include "geometric.h"
#include "matrix_transform.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Transform::Transform()
        : Transform::Transform({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, rex::identity<rex::quaternion>())
    {
    }
    //-------------------------------------------------------------------------
    Transform::Transform(const rex::vec3& position)
        : Transform::Transform(position, {1.0f, 1.0f, 1.0f}, rex::identity<rex::quaternion>())
    {
    }
    //-------------------------------------------------------------------------
    Transform::Transform(const rex::vec3& position, const rex::vec3& scale)
        : Transform::Transform(position, scale, rex::identity<rex::quaternion>())
    {
    }
    //-------------------------------------------------------------------------
    Transform::Transform(const rex::vec3& position, const rex::vec3& scale, const rex::quaternion& rotation)
        : m_position(position)
        , m_scale(scale)
        , m_rotation(rotation)
    {
        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::set_position(const rex::vec3& position)
    {
        if (rex::is_identical(m_position, position))
        {
            return;
        }

        m_position = position;

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::set_scale(const rex::vec3& scale)
    {
        if (rex::is_identical(m_scale, scale))
        {
            return;
        }

        m_scale = scale;

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::set_scale(const float scale)
    {
        if (rex::is_identical(m_scale, rex::vec3(scale, scale, scale)))
        {
            return;
        }

        m_scale = rex::vec3(scale, scale, scale);

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::set_rotation(const rex::quaternion& rotation)
    {
        if (rex::is_identical(m_rotation, rotation))
        {
            return;
        }

        m_rotation = rotation;

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::translate(const rex::vec3& delta)
    {
        m_position += delta;

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::scale(const rex::vec3& delta)
    {
        m_scale += delta;

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::scale(const float delta)
    {
        m_scale += rex::vec3(delta, delta, delta);

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    void Transform::rotate(const float angle, const rex::vec3& axis)
    {
        m_rotation *= glm::angleAxis(angle, axis);

        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    const rex::matrix4& Transform::get_world() const
    {
        return m_world;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& Transform::get_position() const
    {
        return m_position;
    }
    //-------------------------------------------------------------------------
    const rex::vec3& Transform::get_scale() const
    {
        return m_scale;
    }

    //-------------------------------------------------------------------------
    const rex::quaternion& Transform::get_rotation() const
    {
        return m_rotation;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& Transform::get_forward() const
    {
        return m_forward;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& Transform::get_right() const
    {
        return m_right;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& Transform::get_up() const
    {
        return m_up;
    }

    //-------------------------------------------------------------------------
    void Transform::reevaluate()
    {
        update_direction_vectors();
        update_matrices();
    }

    //-------------------------------------------------------------------------
    bool Transform::operator==(const Transform& other) const
    {
        return rex::is_identical(m_position, other.m_position) && rex::is_identical(m_scale, other.m_scale) && rex::is_identical(m_rotation, other.m_rotation);
    }
    //-------------------------------------------------------------------------
    bool Transform::operator!=(const Transform& other) const
    {
        return !(*this == other);
    }

    //-------------------------------------------------------------------------
    void Transform::update_direction_vectors()
    {
        rex::vec3 forward = rex::world_forward<float>();

        m_forward = rex::normalize(rex::vec3(m_rotation * forward));
        m_right = rex::normalize(rex::cross(rex::world_up<float>(), m_forward));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = rex::normalize(rex::cross(m_forward, m_right));
    }

    //-------------------------------------------------------------------------
    void Transform::update_matrices()
    {
        m_world = rex::matrix4(1.0f);

        m_world = rex::translate(m_world, m_position);
        m_world = rex::scale(m_world, m_scale);
        m_world = rex::rotate(m_world, rex::euler_angles(m_rotation).x, rex::pitch_axis<float>());
        m_world = rex::rotate(m_world, rex::euler_angles(m_rotation).y, rex::yaw_axis<float>());
        m_world = rex::rotate(m_world, rex::euler_angles(m_rotation).z, rex::roll_axis<float>());
    }
}