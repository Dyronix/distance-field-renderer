
//
// Basic implementation of an angle in radians by RisingLiberty ( a.k.a. Nick De Breuck )

#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template<typename T>
    constexpr T rad2deg(T angle)
    {
        static_assert(std::numeric_limits<T>::is_iec559, "'degrees' only accept floating-point input");

        return angle * static_cast<T>(57.295779513082320876798154814105);
    }

    class RadAngle
    {
    public:
        //-------------------------------------------------------------------------
        explicit RadAngle();
        explicit RadAngle(float angle);

        RadAngle operator+(const RadAngle& angle) const;
        RadAngle& operator+=(const RadAngle& angle);
        RadAngle operator-(const RadAngle& angle) const;
        RadAngle& operator-=(const RadAngle& angle);
        RadAngle operator*(const RadAngle& angle) const;
        RadAngle& operator*=(const RadAngle& angle);
        RadAngle operator/(const RadAngle& angle) const;
        RadAngle& operator/=(const RadAngle& angle);
        RadAngle operator-() const;

        float get() const;
        float to_degree() const;
        
        bool operator<(const RadAngle angle) const;
        bool operator<=(const RadAngle angle) const;
        bool operator>(const RadAngle angle) const;
        bool operator>=(const RadAngle angle) const;
        bool operator==(const RadAngle angle) const;
        bool operator!=(const RadAngle angle) const;

    private:
        float m_angle;
    };

    //-------------------------------------------------------------------------
    inline rex::RadAngle make_rad_angle(float angle)
    {
        return rex::RadAngle(angle);
    }
    //-------------------------------------------------------------------------
    inline bool is_identical(const rex::RadAngle& src, const rex::RadAngle& dst, double threshold = std::numeric_limits<float>::epsilon())
    {
        return std::abs((dst - src).get()) < threshold;
    }
}

//-------------------------------------------------------------------------
inline rex::RadAngle operator "" _rad(long double angle)
{
    return rex::RadAngle((float)angle);
}
//-------------------------------------------------------------------------
inline rex::RadAngle operator"" _rad(unsigned long long angle)
{
    return rex::RadAngle((float)angle);
}