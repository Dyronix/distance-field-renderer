#pragma once

#include "point.h"

namespace rex
{
    class InputImpl
    {
    public:
        virtual bool is_key_pressed(const uint16 code) const = 0;
        virtual bool is_key_released(const uint16 code) const = 0;

        virtual bool is_mouse_button_pressed(const uint16 button) const = 0;
        virtual bool is_mouse_button_released(const uint16 button) const = 0;

        virtual PointD get_mouse_position() const = 0;

        virtual double get_mouse_x() const = 0;
        virtual double get_mouse_y() const = 0;
    };
}