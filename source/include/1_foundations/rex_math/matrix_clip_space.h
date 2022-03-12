#pragma once

namespace rex
{
    using matrix_qualifier = glm::qualifier;

    //-------------------------------------------------------------------------
    template<typename T>
    glm::mat<4, 4, T, matrix_qualifier::defaultp> ortho(T left, T right, T bottom, T top)
    {
        return glm::ortho(left, right, bottom, top);
    }
    //-------------------------------------------------------------------------
    template<typename T>
    glm::mat<4, 4, T, matrix_qualifier::defaultp> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
    {
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    glm::mat<4, 4, T, matrix_qualifier::defaultp> perspective(T fovy, T aspect, T zNear, T zFar)
    {
        return glm::perspective(fovy, aspect, zNear, zFar);
    }
}