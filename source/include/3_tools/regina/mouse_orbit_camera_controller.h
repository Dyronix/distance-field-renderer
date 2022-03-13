#pragma once

#include "orbit_camera_controller.h"

namespace rex
{
    class InputImpl;
}

namespace regina
{
    class MouseOrbitCameraController : public OrbitCameraController
    {
    public:
        MouseOrbitCameraController(rex::InputImpl* inputInstance, int16 mouseCode, const OrbitCameraDescription& desc);
        ~MouseOrbitCameraController() override;

        void enable_mouse_control();
        void disable_mouse_control();

    protected:
        bool on_can_rotate() const override;

    private:
        rex::InputImpl*  m_input_instance;
        bool    m_mouse_enabled;
        int16   m_mouse_code;
    };
}