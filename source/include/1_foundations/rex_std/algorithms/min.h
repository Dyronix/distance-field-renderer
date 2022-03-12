#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template <typename T>
    constexpr const T& min(const T& left, const T& right) noexcept
    {
        return right < left ? right : left;
    }
    //-------------------------------------------------------------------------
    template <typename T, typename Predicate>
    constexpr const T& min(const T& left, const T& right, Predicate pred) noexcept
    {
        return pred(right, left) ? right : left;
    }
}