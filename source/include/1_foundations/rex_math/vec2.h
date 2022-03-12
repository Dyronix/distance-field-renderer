#pragma once

#include <glm/glm.hpp>

namespace rex
{
    using vector_qualifier = glm::qualifier;

    using vec2 = glm::vec<2, float>;

    using vec2i = glm::vec<2, int>;
    using vec2u = glm::vec<2, unsigned int>;

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    const glm::vec<2, T, Q> zero_vec2()
    {
        return {};
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    T const* value_ptr(glm::vec<2, T, Q> const& v)
    {
        return &(v.x);
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    T* value_ptr(glm::vec<2, T, Q>& v)
    {
        return &(v.x);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    glm::vec<2, T, vector_qualifier::defaultp> make_vec2(T const* const ptr)
    {
        return glm::make_vec2(ptr);
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    glm::vec<2, T, Q> radians(glm::vec<2, T, Q> const& v)
    {
        return glm::vec<2, T, Q>(glm::radians(v.x), glm::radians(v.y));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    glm::vec<2, T, Q> degrees(glm::vec<2, T, Q> const& v)
    {
        return glm::vec<2, T, Q>(glm::degrees(v.x), glm::degrees(v.y));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    bool is_identical(const glm::vec<2, T, Q>& src, const glm::vec<2, T, Q>& dst, T threshold = std::numeric_limits<T>::epsilon())
    {
        return glm::distance(src, dst) < threshold;
    }
}