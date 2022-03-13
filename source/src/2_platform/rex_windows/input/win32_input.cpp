#include "rex_windows_pch.h"

#include "input/win32_input.h"
#include "input/win32_input_controller.h"

namespace rex
{
    namespace win32
    {
        //-------------------------------------------------------------------------
        InputImpl::InputImpl(const Window& window)
            : m_controller(std::make_unique<InputController>(window))
        {
        }
        //-------------------------------------------------------------------------
        InputImpl::~InputImpl() = default;

        //-------------------------------------------------------------------------
        bool InputImpl::is_key_pressed(const uint16 code) const
        {
            return m_controller->is_key_pressed(static_cast<KeyCode>(code));
        }
        //-------------------------------------------------------------------------
        bool InputImpl::is_key_released(const uint16 code) const
        {
            return m_controller->is_key_released(static_cast<KeyCode>(code));
        }

        //-------------------------------------------------------------------------
        bool InputImpl::is_mouse_button_pressed(const uint16 button) const
        {
            return m_controller->is_mouse_button_pressed(static_cast<MouseCode>(button));
        }
        //-------------------------------------------------------------------------
        bool InputImpl::is_mouse_button_released(const uint16 button) const
        {
            return m_controller->is_mouse_button_released(static_cast<MouseCode>(button));
        }

        //-------------------------------------------------------------------------
        PointD InputImpl::get_mouse_position() const
        {
            return m_controller->get_mouse_position();
        }

        //-------------------------------------------------------------------------
        double InputImpl::get_mouse_x() const
        {
            return m_controller->get_mouse_position().x;
        }
        //-------------------------------------------------------------------------
        double InputImpl::get_mouse_y() const
        {
            return m_controller->get_mouse_position().y;
        }
    }
}