#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    inline bool is_identical(const float& src, const float& dst, float threshold = std::numeric_limits<float>::epsilon())
    {
        return std::abs(dst - src) < threshold;
    }
    //-------------------------------------------------------------------------
    inline bool is_identical(const double& src, const double& dst, double threshold = std::numeric_limits<double>::epsilon())
    {
        return std::abs(dst - src) < threshold;
    }
}