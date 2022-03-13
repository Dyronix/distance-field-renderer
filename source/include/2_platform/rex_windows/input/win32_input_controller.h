#pragma once

#include "point.h"

#include "input/keyboard/win32_key_codes.h"
#include "input/mouse/win32_mouse_codes.h"

namespace rex
{
    namespace win32
    {
        class Window;

        class InputController
        {
        public:
            InputController(const Window& window);
            ~InputController();

            PointD get_mouse_position();
            PointD get_mouse_movement() const;

            void show_mouse_cursor(bool show);

            bool is_mouse_button_pressed(MouseCode button) const;
            bool is_mouse_button_released(MouseCode button) const;

            bool is_key_pressed(KeyCode code) const;
            bool is_key_released(KeyCode code) const;

        private:
            const Window& m_window;

            PointD current_mouse_position;
            PointD previous_mouse_position;
        };
    }
}
