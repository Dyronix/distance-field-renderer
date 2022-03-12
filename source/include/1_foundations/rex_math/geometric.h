#pragma once

namespace rex
{
    using vector_size_type = int;
    using vector_qualifier = glm::qualifier;

    //-------------------------------------------------------------------------
    template<vector_size_type L, typename T, vector_qualifier Q>
    glm::vec<L, T, Q> normalize(const glm::vec<L, T, Q>& v)
    {
        return glm::normalize(v);
    }

    //-------------------------------------------------------------------------
    template<vector_size_type L, typename T, vector_qualifier Q>
    T dot(const glm::vec<L, T, Q>& x, const glm::vec<L, T, Q>& y)
    {
        return glm::dot(x, y);
    }

    //-------------------------------------------------------------------------
    template<vector_size_type L, typename T, vector_qualifier Q>
    T length(const glm::vec<L, T, Q>& v)
    {
        return glm::length(v);
    }

    //-------------------------------------------------------------------------
    template<vector_size_type L, typename T, vector_qualifier Q>
    T length2(const glm::vec<L, T, Q>& v)
    {
        return glm::length2(v);
    }

    //-------------------------------------------------------------------------
    template<vector_size_type L, typename T, vector_qualifier Q>
    T distance(const glm::vec<L, T, Q>& p0, const glm::vec<L, T, Q>& p1)
    {
        return glm::distance(p0, p1);
    }
}
