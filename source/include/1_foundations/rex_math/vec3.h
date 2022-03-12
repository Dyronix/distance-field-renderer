#pragma once

namespace rex
{
    using vector_qualifier = glm::qualifier;

    using vec3 = glm::vec<3, float>;

    using vec3i = glm::vec<3, int>;
    using vec3u = glm::vec<3, unsigned int>;

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    const glm::vec<3, T, Q> zero_vec3()
    {
        return {};
    }

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    glm::vec<3, T, Q> cross(const glm::vec<3, T, Q>& x, const glm::vec<3, T, Q>& y)
    {
        return glm::cross(x, y);
    }

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    T const* value_ptr(glm::vec<3, T, Q> const& v)
    {
        return &(v.x);
    }

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    T* value_ptr(glm::vec<3, T, Q>& v)
    {
        return &(v.x);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    glm::vec<3, T, vector_qualifier::defaultp> make_vec3(T const* const ptr)
    {
        return glm::make_vec3(ptr);
    }

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    glm::vec<3, T, Q> radians(glm::vec<3, T, Q> const& v)
    {
        return glm::vec<3, T, Q>(glm::radians(v.x), glm::radians(v.y), glm::radians(v.z));
    }

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    glm::vec<3, T, Q> degrees(glm::vec<3, T, Q> const& v)
    {
        return glm::vec<3, T, Q>(glm::degrees(v.x), glm::degrees(v.y), glm::degrees(v.z));
    }

    //-------------------------------------------------------------------------
    template <typename T, vector_qualifier Q>
    bool is_identical(const glm::vec<3, T, Q>& src, const glm::vec<3, T, Q>& dst, T threshold = std::numeric_limits<T>::epsilon())
    {
        return glm::distance(src, dst) < threshold;
    }
}