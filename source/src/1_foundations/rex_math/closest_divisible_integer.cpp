#include "rex_math_pch.h"

#include "closest_divisible_integer.h"

namespace rex
{
    //-------------------------------------------------------------------------
    int32 closest_divisible_integer_greater_than(int32 numerator, int32 denominator)
    {
        return (numerator + denominator) - (numerator % denominator);
    }
    //-------------------------------------------------------------------------
    int32 closest_divisible_integer_smaller_than(int32 numerator, int32 denominator)
    {
        return numerator - (numerator % denominator);
    }
    //-------------------------------------------------------------------------
    int32 closest_divisible_integer(int32 numerator, int32 denominator)
    {
        int32 c1 = numerator - (numerator % denominator);
        int32 c2 = (numerator + denominator) - (numerator % denominator);

        if (numerator - c1 > c2 - numerator)
        {
            return c2;
        }

        return c1;
    }
}