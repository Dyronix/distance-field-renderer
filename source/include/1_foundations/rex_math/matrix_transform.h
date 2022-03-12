#pragma once

namespace rex
{
    using matrix_size_type = int;
    using matrix_qualifier = glm::qualifier;

    //-------------------------------------------------------------------------
    template<typename T, matrix_qualifier Q>
    glm::mat<4, 4, T, Q> look_at(const glm::vec<3, T, Q>& eye, const glm::vec<3, T, Q>& center, const glm::vec<3, T, Q>& up)
    {
        return glm::lookAt(eye, center, up);
    }

    //-------------------------------------------------------------------------
    template<typename T, matrix_qualifier Q>
    glm::mat<4, 4, T, Q> scale(glm::mat<4, 4, T, Q> const& m, const glm::vec<3, T, Q>& v)
    {
        return glm::scale(m, v);
    }

    //-------------------------------------------------------------------------
    template<typename T, matrix_qualifier Q>
    glm::mat<4, 4, T, Q> rotate(glm::mat<4, 4, T, Q> const& m, T angle, const glm::vec<3, T, Q>& v)
    {
        return glm::rotate(m, angle, v);
    }

    //-------------------------------------------------------------------------
    template<typename T, matrix_qualifier Q>
    glm::mat<4, 4, T, Q> translate(glm::mat<4, 4, T, Q> const& m, const glm::vec<3, T, Q>& v)
    {
        return glm::translate(m, v);
    }
}