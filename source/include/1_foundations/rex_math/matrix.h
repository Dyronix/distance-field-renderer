#pragma once

namespace rex
{
    using matrix_column = int;
    using matrix_row = int;
    using matrix_qualifier = glm::qualifier;

    using matrix2 = glm::mat<2, 2, float>;
    using matrix3 = glm::mat<3, 3, float>;
    using matrix4 = glm::mat<4, 4, float>;

    using matrix2i = glm::mat<2, 2, int>;
    using matrix3i = glm::mat<3, 3, int>;
    using matrix4i = glm::mat<4, 4, int>;

    //-------------------------------------------------------------------------
    template <typename T>
    T identity()
    {
        return glm::identity<T>();
    }

    //-------------------------------------------------------------------------
    template <matrix_column C, matrix_row R, typename T, matrix_qualifier Q>
    glm::mat<C, R, T, Q> inverse(glm::mat<C, R, T, Q> const& m)
    {
        return glm::inverse(m);
    }

    //-------------------------------------------------------------------------
    template <typename T, matrix_qualifier Q>
    T const* value_ptr(glm::mat<2, 2, T, Q> const& m)
    {
        return &(m[0].x);
    }
    //-------------------------------------------------------------------------
    template <typename T, matrix_qualifier Q>
    T* value_ptr(glm::mat<2, 2, T, Q>& m)
    {
        return &(m[0].x);
    }

    //-------------------------------------------------------------------------
    template <typename T, matrix_qualifier Q>
    T const* value_ptr(glm::mat<3, 3, T, Q> const& m)
    {
        return &(m[0].x);
    }

    //-------------------------------------------------------------------------
    template <typename T, matrix_qualifier Q>
    T* value_ptr(glm::mat<3, 3, T, Q>& m)
    {
        return &(m[0].x);
    }

    //-------------------------------------------------------------------------
    template <typename T, matrix_qualifier Q>
    T const* value_ptr(glm::mat<4, 4, T, Q> const& m)
    {
        return &(m[0].x);
    }

    //-------------------------------------------------------------------------
    template <typename T, matrix_qualifier Q>
    T* value_ptr(glm::mat<4, 4, T, Q>& m)
    {
        return &(m[0].x);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    glm::mat<2, 2, T, matrix_qualifier::defaultp> make_mat2x2(T const* const ptr)
    {
        return glm::make_mat2x2(ptr);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    glm::mat<3, 3, T, matrix_qualifier::defaultp> make_mat3x3(T const* const ptr)
    {
        return glm::make_mat3x3(ptr);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    glm::mat<4, 4, T, matrix_qualifier::defaultp> make_mat4x4(T const* const ptr)
    {
        return glm::make_mat4x4(ptr);
    }
}