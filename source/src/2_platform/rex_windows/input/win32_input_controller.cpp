#include "rex_windows_pch.h"

#include "input/win32_input_controller.h"

#include "win32_window.h"

#include <SDL.h>

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        InputController::InputController(const Window& window)
            : m_window(window)
        {
        }
        //-------------------------------------------------------------------------
        InputController::~InputController() = default;

        //-------------------------------------------------------------------------
        PointD InputController::get_mouse_position()
        {
            int32 x, y;
            SDL_GetMouseState(&x, &y);

            previous_mouse_position = current_mouse_position;
            current_mouse_position = {(double)x, (double)y};

            return current_mouse_position;
        }
        //-------------------------------------------------------------------------
        PointD InputController::get_mouse_movement() const
        {
            PointD delta;

            delta.x = current_mouse_position.x - previous_mouse_position.x;
            delta.y = current_mouse_position.y - previous_mouse_position.y;

            return delta;
        }

        //-------------------------------------------------------------------------
        void InputController::show_mouse_cursor(bool show)
        {
            if (show)
            {
                SDL_ShowCursor(1);
            }
            else
            {
                SDL_ShowCursor(0);
            }
        }

        //-------------------------------------------------------------------------
        bool InputController::is_mouse_button_pressed(MouseCode button) const
        {
            SDL_PumpEvents();
            switch (button)
            {
                case MouseCode::BUTTON_LEFT: return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
                case MouseCode::BUTTON_MIDDLE: return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
                case MouseCode::BUTTON_RIGHT: return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
                case MouseCode::BUTTON_X1: return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X1);
                case MouseCode::BUTTON_X2: return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X2);
            }

            return false;
        }
        //-------------------------------------------------------------------------
        bool InputController::is_mouse_button_released(MouseCode button) const
        {
            SDL_PumpEvents();
            switch (button)
            {
                case MouseCode::BUTTON_LEFT: return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
                case MouseCode::BUTTON_MIDDLE: return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
                case MouseCode::BUTTON_RIGHT: return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
                case MouseCode::BUTTON_X1: return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X1));
                case MouseCode::BUTTON_X2: return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X2));
            }

            return false;
        }

        //-------------------------------------------------------------------------
        bool InputController::is_key_pressed(KeyCode code) const
        {
            const uint8* keystate = SDL_GetKeyboardState(nullptr);
            if (keystate == nullptr || std::strcmp((const char*)keystate, "") == 0)
            {
                return false;
            }

            return keystate[(int32)code];
        }
        //-------------------------------------------------------------------------
        bool InputController::is_key_released(KeyCode code) const
        {
            const uint8* keystate = SDL_GetKeyboardState(nullptr);
            if (keystate == nullptr || std::strcmp((const char*)keystate, "") == 0)
            {
                return false;
            }

            return !(keystate[(int32)code]);
        }
    }
}