#pragma once

#include "algorithms/lerp.h"

namespace rex
{
    using vector_size_type = int;
    using vector_qualifier = glm::qualifier;

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    glm::vec<2, T, Q> lerp(const glm::vec<2, T, Q>& a, const glm::vec<2, T, Q>& b, T t)
    {
        glm::vec<2, T, Q> res;

        res.x = lerp(a.x, b.x, t);
        res.y = lerp(a.y, b.y, t);

        return res;
    }
    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    glm::vec<3, T, Q> lerp(const glm::vec<3, T, Q>& a, const glm::vec<3, T, Q>& b, T t)
    {
        glm::vec<3, T, Q> res;

        res.x = lerp(a.x, b.x, t);
        res.y = lerp(a.y, b.y, t);
        res.z = lerp(a.z, b.z, t);

        return res;
    }
    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    glm::vec<4, T, Q> lerp(const glm::vec<4, T, Q>& a, const glm::vec<4, T, Q>& b, T t)
    {
        glm::vec<4, T, Q> res;

        res.x = lerp(a.x, b.x, t);
        res.y = lerp(a.y, b.y, t);
        res.y = lerp(a.z, b.y, t);
        res.y = lerp(a.w, b.y, t);

        return res;
    }

    //-------------------------------------------------------------------------
    template <vector_size_type L, typename T, vector_qualifier Q>
    glm::vec<L, T, Q> normalize(const glm::vec<L, T, Q>& v)
    {
        return glm::normalize(v);
    }

    //-------------------------------------------------------------------------
    template <vector_size_type L, typename T, vector_qualifier Q>
    T dot(const glm::vec<L, T, Q>& x, const glm::vec<L, T, Q>& y)
    {
        return glm::dot(x, y);
    }

    //-------------------------------------------------------------------------
    template <vector_size_type L, typename T, vector_qualifier Q>
    T length(const glm::vec<L, T, Q>& v)
    {
        return glm::length(v);
    }

    //-------------------------------------------------------------------------
    template <vector_size_type L, typename T, vector_qualifier Q>
    T length2(const glm::vec<L, T, Q>& v)
    {
        return glm::length2(v);
    }

    //-------------------------------------------------------------------------
    template <vector_size_type L, typename T, vector_qualifier Q>
    T distance(const glm::vec<L, T, Q>& p0, const glm::vec<L, T, Q>& p1)
    {
        return glm::distance(p0, p1);
    }
}
