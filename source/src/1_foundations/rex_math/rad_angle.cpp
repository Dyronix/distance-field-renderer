#include "rex_math_pch.h"

#include "rad_angle.h"

namespace rex
{
        //-------------------------------------------------------------------------
        RadAngle::RadAngle()
            : m_angle(0.0f)
        {}
        //-------------------------------------------------------------------------
        RadAngle::RadAngle(float angle)
            : m_angle(angle)
        {}

        //-------------------------------------------------------------------------
        RadAngle RadAngle::operator+(const RadAngle& angle) const
        {
            return RadAngle(m_angle + angle.m_angle);
        }
        //-------------------------------------------------------------------------
        RadAngle& RadAngle::operator+=(const RadAngle& angle)
        {
            m_angle += angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        RadAngle RadAngle::operator-(const RadAngle& angle) const
        {
            return RadAngle(m_angle - angle.m_angle);
        }
        //-------------------------------------------------------------------------
        RadAngle& RadAngle::operator-=(const RadAngle& angle)
        {
            m_angle -= angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        RadAngle RadAngle::operator*(const RadAngle& angle) const
        {
            return RadAngle(m_angle * angle.m_angle);
        }
        //-------------------------------------------------------------------------
        RadAngle& RadAngle::operator*=(const RadAngle& angle)
        {
            m_angle *= angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        RadAngle RadAngle::operator/(const RadAngle& angle) const
        {
            return RadAngle(m_angle / angle.m_angle);
        }
        //-------------------------------------------------------------------------
        RadAngle& RadAngle::operator/=(const RadAngle& angle)
        {
            m_angle /= angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        RadAngle RadAngle::operator-() const
        {
            return RadAngle(-m_angle);
        }

        //-------------------------------------------------------------------------
        float RadAngle::get() const
        {
            return m_angle;
        }
        //-------------------------------------------------------------------------
        float RadAngle::to_degree() const
        {
            return rex::rad2deg(m_angle);
        }

        //-------------------------------------------------------------------------
        bool RadAngle::operator<(const RadAngle angle) const
        {
            return m_angle < angle.get();
        }
        //-------------------------------------------------------------------------
        bool RadAngle::operator<=(const RadAngle angle) const
        {
            return m_angle <= angle.get();
        }

        //-------------------------------------------------------------------------
        bool RadAngle::operator>(const RadAngle angle) const
        {
            return m_angle > angle.get();
        }
        //-------------------------------------------------------------------------
        bool RadAngle::operator>=(const RadAngle angle) const
        {
            return m_angle >= angle.get();
        }

        //-------------------------------------------------------------------------
        bool RadAngle::operator==(const RadAngle angle) const
        {
            return m_angle == angle.get();
        }
        //-------------------------------------------------------------------------
        bool RadAngle::operator!=(const RadAngle angle) const
        {
            return m_angle != angle.get();
        }
}