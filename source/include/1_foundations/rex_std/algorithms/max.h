#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template <typename T>
    constexpr const T& max(const T& left, const T& right) noexcept
    {
        return left < right ? right : left;
    }
    //-------------------------------------------------------------------------
    template <typename T, typename Predicate>
    constexpr const T& max(const T& left, const T& right, Predicate pred) noexcept
    {
        return pred(left, right) ? right : left;
    }
}