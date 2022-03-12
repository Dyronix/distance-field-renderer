
//
// Basic implementation of an angle in degrees by RisingLiberty ( a.k.a. Nick De Breuck )

#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template <typename T>
    constexpr T deg2rad(T angle)
    {
        static_assert(std::numeric_limits<T>::is_iec559, "'radians' only accept floating-point input");

        return angle * static_cast<T>(0.01745329251994329576923690768489);
    }

    class DegAngle
    {
    public:
        explicit DegAngle();
        explicit DegAngle(float angle);

        DegAngle operator+(const DegAngle& angle) const;
        DegAngle& operator+=(const DegAngle& angle);
        DegAngle operator-(const DegAngle& angle) const;
        DegAngle& operator-=(const DegAngle& angle);
        DegAngle operator*(const DegAngle& angle) const;
        DegAngle& operator*=(const DegAngle& angle);
        DegAngle operator/(const DegAngle& angle) const;
        DegAngle& operator/=(const DegAngle& angle);
        DegAngle operator-() const;

        float get() const;
        float to_rad() const;

        bool operator<(const DegAngle angle) const;
        bool operator<=(const DegAngle angle) const;
        bool operator>(const DegAngle angle) const;
        bool operator>=(const DegAngle angle) const;
        bool operator==(const DegAngle angle) const;
        bool operator!=(const DegAngle angle) const;

    private:
        float m_angle;
    };

    //-------------------------------------------------------------------------
    inline DegAngle make_deg_angle(float angle)
    {
        return DegAngle(angle);
    }

    //-------------------------------------------------------------------------
    inline bool is_identical(const DegAngle& src, const DegAngle& dst, double threshold = std::numeric_limits<float>::epsilon())
    {
        return std::abs((dst - src).get()) < threshold;
    }
}

//-------------------------------------------------------------------------
inline rex::DegAngle operator"" _deg(long double angle)
{
    return rex::DegAngle((float)angle);
}
//-------------------------------------------------------------------------
inline rex::DegAngle operator"" _deg(unsigned long long angle)
{
    return rex::DegAngle((float)angle);
}