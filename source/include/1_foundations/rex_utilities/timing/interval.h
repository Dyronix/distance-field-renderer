#pragma once

#include "timing/tickcount.h"

namespace rex
{
    class Interval
    {
    public:
        //-------------------------------------------------------------------------
        Interval()
            : m_initial(get_tick_count())
        {
        }
        //-------------------------------------------------------------------------
        ~Interval() = default;

        //-------------------------------------------------------------------------
        inline uint64 value() const
        {
            return get_tick_count() - m_initial;
        }

    private:
        uint64 m_initial;
    };
}