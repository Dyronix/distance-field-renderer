#pragma once

namespace rex
{
    using quat_qualifier = glm::qualifier;

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    T angle(glm::qua<T, Q> const& x)
    {
        return glm::angle(x);
    }

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    glm::vec<3, T, Q> axis(glm::qua<T, Q> const& x)
    {
        return glm::axis(x);
    }

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    glm::qua<T, Q> angle_axis(T const& angle, glm::vec<3, T, Q> const& v)
    {
        return glm::angleAxis(angle, v);
    }
}