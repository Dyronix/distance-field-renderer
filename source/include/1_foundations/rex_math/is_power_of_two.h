#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template <typename TInteger, std::enable_if_t<std::is_integral<TInteger>::value, bool> = true>
    bool is_power_of_two(TInteger value)
    {
        return value && !(value & (value-1));
    }
    
    //-------------------------------------------------------------------------
    template <typename TFloat, std::enable_if_t<std::is_floating_point<TFloat>::value, bool> = true>
    bool is_power_of_two(TFloat value)
    {
        return std::ceil(std::log2(value)) == std::floor(std::log2(value));
    }
}