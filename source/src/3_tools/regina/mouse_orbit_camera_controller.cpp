#include "regina_pch.h"

#include "mouse_orbit_camera_controller.h"

#include "input.h"

namespace regina
{
    //-------------------------------------------------------------------------
    MouseOrbitCameraController::MouseOrbitCameraController(rex::InputImpl* inputInstance, int16 mouseCode, const OrbitCameraDescription& desc)
        : OrbitCameraController(desc)
        , m_input_instance(inputInstance)
        , m_mouse_code(mouseCode)
        , m_mouse_enabled(true)
    {
    }

    //-------------------------------------------------------------------------
    MouseOrbitCameraController::~MouseOrbitCameraController()
    {
    }

    //-------------------------------------------------------------------------
    void MouseOrbitCameraController::enable_mouse_control()
    {
        m_mouse_enabled = true;
    }

    //-------------------------------------------------------------------------
    void MouseOrbitCameraController::disable_mouse_control()
    {
        m_mouse_enabled = false;
    }

    //-------------------------------------------------------------------------
    bool MouseOrbitCameraController::on_can_rotate() const
    {
        if (m_mouse_enabled)
        {
            return m_input_instance->is_mouse_button_pressed(m_mouse_code);
        }

        return true;
    }
}