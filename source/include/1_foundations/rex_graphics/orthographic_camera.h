#pragma once

namespace rex
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void set_position(const rex::vec3& position);
        void set_rotation(float rotation);

        void set_projection(float left, float right, float bottom, float top);

        const rex::vec3& get_position() const;
        float get_rotation() const;

        const rex::matrix4& get_inverse_view_projection_matrix() const;
        const rex::matrix4& get_view_projection_matrix() const;
        const rex::matrix4& get_projection_matrix() const;
        const rex::matrix4& get_view_matrix() const;

    private:
        void recalculate_view_matrix();

        rex::matrix4 m_inv_view_projection_matrix;
        rex::matrix4 m_view_projection_matrix;
        rex::matrix4 m_projection_matrix;
        rex::matrix4 m_view_matrix;

        rex::vec3 m_position;

        float m_rotation;
    };
}