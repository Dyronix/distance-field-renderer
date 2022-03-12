#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template<typename T, class Compare>
    constexpr const T& clamp(const T& value, const T& min, const T& max, Compare comp)
    {
        assert( !comp(max, min) );

        return comp(value, min) ? min : comp(max, value) ? max : value;
    }

    //-------------------------------------------------------------------------
    template<class T>
    constexpr const T& clamp( const T& value, const T& min, const T& max )
    {
        return rex::clamp(value, min, max, std::less<T>{});
    }
}