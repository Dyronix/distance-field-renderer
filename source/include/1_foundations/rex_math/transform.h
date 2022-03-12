#pragma once

#include "vec3.h"
#include "quaternion.h"
#include "matrix.h"

namespace rex
{
    class Transform
    {
    public:
        Transform();
        Transform(const rex::vec3& position);
        Transform(const rex::vec3& position, const rex::vec3& scale);
        Transform(const rex::vec3& position, const rex::vec3& scale, const rex::quaternion& rotation);

        void set_position(const rex::vec3& position);
        void set_scale(const rex::vec3& scale);
        void set_scale(const float scale);
        void set_rotation(const rex::quaternion& rotation);

        void translate(const rex::vec3& delta);

        void scale(const rex::vec3& delta);
        void scale(const float delta);

        void rotate(const float angle, const rex::vec3& axis);

        const rex::matrix4& get_world() const;

        const rex::vec3& get_position() const;
        const rex::vec3& get_scale() const;
        const rex::quaternion& get_rotation() const;

        const rex::vec3& get_forward() const;
        const rex::vec3& get_right() const;
        const rex::vec3& get_up() const;

        void reevaluate();

        bool operator==(const Transform& other) const;
        bool operator!=(const Transform& other) const;

    private:
        void update_direction_vectors();
        void update_matrices();

        rex::matrix4 m_world;

        rex::vec3 m_position;
        rex::vec3 m_scale;

        rex::quaternion m_rotation;

        rex::vec3 m_forward;
        rex::vec3 m_right;
        rex::vec3 m_up;
    };
}
