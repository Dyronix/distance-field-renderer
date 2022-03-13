#pragma once

#include "input.h"

#include "patterns/singleton.h"

namespace rex
{
    namespace win32
    {
        class Window;
        class InputController;

        class InputImpl : public rex::InputImpl
        {
        public:
            InputImpl(const Window& window);
            ~InputImpl();

            bool is_key_pressed(const uint16 code) const override;
            bool is_key_released(const uint16 code) const override;

            bool is_mouse_button_pressed(const uint16 button) const override;
            bool is_mouse_button_released(const uint16 button) const override;

            PointD get_mouse_position() const override;

            double get_mouse_x() const override;
            double get_mouse_y() const override;

        private:
            std::unique_ptr<InputController> m_controller;
        };

        class Input : public StaticSingleton<rex::win32::InputImpl>
        {
        };
    }
}