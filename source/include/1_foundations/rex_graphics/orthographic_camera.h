#pragma once

namespace rex
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void setPosition(const rex::vec3& position);
        void setRotation(float rotation);

        void setProjection(float left, float right, float bottom, float top);

        const rex::vec3& getPosition() const;
        float getRotation() const;

        const rex::matrix4& getInverseViewProjectionMatrix() const;
        const rex::matrix4& getViewProjectionMatrix() const;
        const rex::matrix4& getProjectionMatrix() const;
        const rex::matrix4& getViewMatrix() const;


    private:
        void recalculateViewMatrix();

        rex::matrix4 m_inv_view_projection_matrix;
        rex::matrix4 m_view_projection_matrix;
        rex::matrix4 m_projection_matrix;
        rex::matrix4 m_view_matrix;

        rex::vec3 m_position;

        float m_rotation;
    };
}