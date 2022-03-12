#pragma once

#include <glm/glm.hpp>

#pragma warning( push )
#pragma warning( disable : 4201 )
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/quaternion.hpp>
    #include <glm/gtx/string_cast.hpp>
    #include <glm/gtc/matrix_access.hpp>
#pragma warning( pop )

namespace rex
{
    using vector_size_type = int;
    using vector_qualifier = glm::qualifier;

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    glm::vec<3, T, Q> world_up()
    {
        return glm::vec<3, T, Q>(T(0.0), T(1.0), T(0.0));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    glm::vec<3, T, Q> world_right()
    {
        return glm::vec<3, T, Q>(T(1.0), T(0.0), T(0.0));
    }
    
    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    glm::vec<3, T, Q> world_forward()
    {
        return glm::vec<3, T, Q>(T(0.0), T(0.0), T(1.0));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    glm::vec<3, T, Q> pitch_axis()
    {
        return glm::vec<3, T, Q>(T(1.0), T(0.0), T(0.0));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    glm::vec<3, T, Q> yaw_axis()
    {
        return glm::vec<3, T, Q>(T(0.0), T(1.0), T(0.0));
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q = vector_qualifier::packed_highp>
    glm::vec<3, T, Q> roll_axis()
    {
        return glm::vec<3, T, Q>(T(0.0), T(0.0), T(1.0));
    }

    //-------------------------------------------------------------------------
    template<vector_size_type L, typename T, vector_qualifier Q>
    glm::vec<L, T, Q> abs(glm::vec<L, T, Q> const& x)
    {
        return glm::abs(x);
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    float mean(const glm::vec<2, T, Q>& v)
    {
        return (v.x + v.y) * 0.5f;
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    float mean(const glm::vec<3, T, Q>& v)
    {
        return (v.x + v.y + v.z) / 3.0f;
    }

    //-------------------------------------------------------------------------
    template<typename T, vector_qualifier Q>
    float mean(const glm::vec<4, T, Q>& v)
    {
        return (v.x + v.y + v.z + v.w) * 0.25f;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    T pi()
    {
        assert(std::numeric_limits<T>::is_iec559 && "'pi' only accepts floating-point inputs");
        return static_cast<T>(3.14159265358979323846264338327950288);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    T row(T const& m, vector_size_type index, typename T::row_type const& x)
    {
        assert(index >= 0 && index < m[0].length());

        T result = m;
        for (vector_size_type i = 0; i < m.length(); ++i)
            result[i][index] = x[i];
        return result;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    typename T::row_type row(T const& m, vector_size_type index)
    {
        assert(index >= 0 && index < m[0].length());

        typename T::row_type result(0);
        for (vector_size_type i = 0; i < m.length(); ++i)
            result[i] = m[i][index];
        return result;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    T column(T const& m, vector_size_type index, typename T::col_type const& x)
    {
        assert(index >= 0 && index < m.length());

        T result = m;
        result[index] = x;
        return result;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    typename T::col_type column(T const& m, vector_size_type index)
    {
        assert(index >= 0 && index < m.length());

        return m[index];
    }

    //-------------------------------------------------------------------------
    template<class matType>
    std::string to_string(matType const& x)
    {
        return glm::to_string(x);
    }
}