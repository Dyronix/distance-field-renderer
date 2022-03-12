#pragma once

namespace rex
{
    using quat_qualifier = glm::qualifier;

    using quaternion_low_f = glm::qua<float, quat_qualifier::packed_lowp>;
    using quaternion_med_f = glm::qua<float, quat_qualifier::packed_mediump>;
    using quaternion_high_f = glm::qua<float, quat_qualifier::packed_highp>;

    using quaternion_low_d = glm::qua<double, quat_qualifier::packed_lowp>;
    using quaternion_med_d = glm::qua<double, quat_qualifier::packed_mediump>;
    using quaternion_high_d = glm::qua<double, quat_qualifier::packed_highp>;

    using quaternion = quaternion_high_f;

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    bool is_identical(const glm::qua<T, Q>& src, const glm::qua<T, Q>& dst, float threshold = std::numeric_limits<float>::epsilon())
    {
        return 1.0f - std::abs(glm::dot(src, dst)) < threshold;
    }

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    glm::vec<3, T, Q> euler_angles(glm::qua<T, Q> const& x)
    {
        return glm::vec<3, T, Q>(glm::pitch(x), glm::yaw(x), glm::roll(x));
    }

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    T roll(glm::qua<T, Q> const& q)
    {
        return glm::roll(q);
    }

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    T pitch(glm::qua<T, Q> const& q)
    {
        return glm::pitch(q);
    }

    //-------------------------------------------------------------------------
    template <typename T, quat_qualifier Q>
    T yaw(glm::qua<T, Q> const& q)
    {
        return glm::yaw(q);
    }
}