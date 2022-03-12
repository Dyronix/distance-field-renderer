#include "rex_math_pch.h"

#include "deg_angle.h"

namespace rex
{
        //-------------------------------------------------------------------------
        DegAngle::DegAngle()
            : m_angle(0.0f)
        {}
        //-------------------------------------------------------------------------
        DegAngle::DegAngle(float angle)
            : m_angle(angle)
        {}

        //-------------------------------------------------------------------------
        DegAngle DegAngle::operator+(const DegAngle& angle) const
        {
            return DegAngle(m_angle + angle.m_angle);
        }
        //-------------------------------------------------------------------------
        DegAngle& DegAngle::operator+=(const DegAngle& angle)
        {
            m_angle += angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        DegAngle DegAngle::operator-(const DegAngle& angle) const
        {
            return DegAngle(m_angle - angle.m_angle);
        }
        //-------------------------------------------------------------------------
        DegAngle& DegAngle::operator-=(const DegAngle& angle)
        {
            m_angle -= angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        DegAngle DegAngle::operator*(const DegAngle& angle) const
        {
            return DegAngle(m_angle * angle.m_angle);
        }
        //-------------------------------------------------------------------------
        DegAngle& DegAngle::operator*=(const DegAngle& angle)
        {
            m_angle *= angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        DegAngle DegAngle::operator/(const DegAngle& angle) const
        {
            return DegAngle(m_angle / angle.m_angle);
        }
        //-------------------------------------------------------------------------
        DegAngle& DegAngle::operator/=(const DegAngle& angle)
        {
            m_angle /= angle.m_angle;
            return *this;
        }

        //-------------------------------------------------------------------------
        DegAngle DegAngle::operator-() const
        {
            return DegAngle(-m_angle);
        }

        //-------------------------------------------------------------------------
        float DegAngle::get() const
        {
            return m_angle;
        }
        //-------------------------------------------------------------------------
        float DegAngle::to_rad() const
        {
            return rex::deg2rad(m_angle);
        }

        //-------------------------------------------------------------------------
        bool DegAngle::operator<(const DegAngle angle) const
        {
            return m_angle < angle.get();
        }
        //-------------------------------------------------------------------------
        bool DegAngle::operator<=(const DegAngle angle) const
        {
            return m_angle <= angle.get();
        }

        //-------------------------------------------------------------------------
        bool DegAngle::operator>(const DegAngle angle) const
        {
            return m_angle > angle.get();
        }
        //-------------------------------------------------------------------------
        bool DegAngle::operator>=(const DegAngle angle) const
        {
            return m_angle >= angle.get();
        }

        //-------------------------------------------------------------------------
        bool DegAngle::operator==(const DegAngle angle) const
        {
            return m_angle == angle.get();
        }
        //-------------------------------------------------------------------------
        bool DegAngle::operator!=(const DegAngle angle) const
        {
            return m_angle != angle.get();
        }
}