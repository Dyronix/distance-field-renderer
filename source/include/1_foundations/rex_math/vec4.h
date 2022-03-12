#pragma once

namespace rex
{
    using vector_qualifier = glm::qualifier;

    using vec4 = glm::vec<4, float>;

    using vec4i = glm::vec<4, int>;
    using vec4u = glm::vec<4, unsigned int>;

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    const glm::vec<4, T, Q> zero_vec4()
    {
        return {};
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    T const* value_ptr(glm::vec<4, T, Q> const& v)
    {
        return &(v.x);
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    T* value_ptr(glm::vec<4, T, Q>& v)
    {
        return &(v.x);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    glm::vec<4, T, vector_qualifier::defaultp> make_vec4(T const* const ptr)
    {
        return glm::make_vec4(ptr);
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    glm::vec<4, T, Q> radians(glm::vec<4, T, Q> const& v)
    {
        return glm::vec<4, T, Q>(glm::radians(v.x), glm::radians(v.y), glm::radians(v.z), glm::radians(v.w));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    glm::vec<4, T, Q> degrees(glm::vec<4, T, Q> const& v)
    {
        return glm::vec<4, T, Q>(glm::degrees(v.x), glm::degrees(v.y), glm::degrees(v.z), glm::degrees(v.w));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    bool is_identical(const glm::vec<4, T, Q>& src, const glm::vec<4, T, Q>& dst, T threshold = std::numeric_limits<T>::epsilon())
    {
        return glm::distance(src, dst) < threshold;
    }
}