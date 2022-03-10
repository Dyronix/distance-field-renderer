#pragma once

#include "timing/tickcount.h"

namespace rex
{
    class Interval
    {
    public:
        //-------------------------------------------------------------------------
        Interval()
            : m_initial(getTickCount())
        {
        }
        //-------------------------------------------------------------------------
        ~Interval() = default;

        //-------------------------------------------------------------------------
        inline uint64 value() const
        {
            return getTickCount() - m_initial;
        }

    private:
        uint64 m_initial;
    };
}