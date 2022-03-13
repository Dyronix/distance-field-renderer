#include "regina_pch.h"

#include "orbit_camera_controller.h"

#include "input/mouse_move.h"
#include "input/mouse_scroll.h"

#include "event_dispatcher.h"

#include "transform.h"

namespace regina
{
    //-------------------------------------------------------------------------
    rex::vec3 get_orbit_angles_from_camera_rotation(const rex::quaternion& cameraRotation)
    {
        return rex::vec3(rex::pitch(cameraRotation), rex::yaw(cameraRotation), rex::roll(cameraRotation));
    }

    //-------------------------------------------------------------------------
    OrbitCameraController::OrbitCameraController(const OrbitCameraDescription& desc)
        : m_orbit_camera(desc)
        , m_focus_controller(desc.focus_settings)
        , m_orbit_controller(get_orbit_angles_from_camera_rotation(desc.camera_settings.camera_rotation), desc.orbit_settings)
        , m_vertical_mouse_movement(0.0f)
        , m_horizontal_mouse_movement(0.0f)
    {
        if (!desc.camera_settings.can_rotate_pitch && !desc.camera_settings.can_rotate_yaw && !desc.camera_settings.can_zoom)
        {
            R_WARN("Camera movement is disabled!");
        }
    }

    //-------------------------------------------------------------------------
    OrbitCameraController::~OrbitCameraController()
    {
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::enable()
    {
        m_orbit_camera.enable();
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::disable()
    {
        m_orbit_camera.disable();
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::reset()
    {
        m_focus_controller.reset(m_orbit_camera.get_focus_settings());
        m_orbit_controller.reset(get_orbit_angles_from_camera_rotation(m_orbit_camera.get_initial_camera_rotation()), m_orbit_camera.get_orbit_settings());

        m_orbit_camera.reset();

        orbit(m_orbit_camera.get_initial_camera_rotation());
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::on_update(const rex::FrameInfo& info)
    {
        UNUSED_PARAM(info);

        if (!m_orbit_camera.can_rotate())
        {
            return;
        }

        orbit();

        m_horizontal_mouse_movement = 0.0f;
        m_vertical_mouse_movement = 0.0f;
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::on_event(rex::events::Event& evt)
    {
        rex::events::EventDispatcher dispatcher(evt);
        dispatcher.dispatch<rex::events::MouseScroll>([&](const rex::events::MouseScroll& scrollEvent)
                                                      {
                                                          return on_mouse_scrolled(scrollEvent);
                                                      });
        dispatcher.dispatch<rex::events::MouseMove>([&](const rex::events::MouseMove& moveEvent)
                                                    {
                                                        return on_mouse_moved(moveEvent);
                                                    });
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_camera(rex::Camera* inCamera)
    {
        m_orbit_camera.set_camera(inCamera);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_camera_transform(rex::Transform* inTransform)
    {
        m_orbit_camera.set_camera_transform(inTransform);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_focus(const rex::vec3& target)
    {
        if (rex::is_identical(target, m_orbit_camera.get_focus()))
        {
            return;
        }

        m_orbit_camera.set_focus(target);

        orbit(m_orbit_camera.get_camera_transform()->get_rotation());
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::set_focus_distance(float distance)
    {
        if (rex::is_identical(distance, m_orbit_camera.get_focus_distance()))
        {
            return;
        }

        m_orbit_camera.set_focus_distance(distance);

        orbit(m_orbit_camera.get_camera_transform()->get_rotation());
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::set_focus_range(const rex::MinMax<float> distance)
    {
        if (rex::is_identical(distance.minimum, m_orbit_camera.get_min_focus_distance()) && rex::is_identical(distance.maximum, m_orbit_camera.get_max_focus_distance()))
        {
            return;
        }

        m_orbit_camera.set_min_focus_distance(distance.minimum);
        m_orbit_camera.set_max_focus_distance(distance.maximum);

        orbit(m_orbit_camera.get_camera_transform()->get_rotation());
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_rotation_speed(float rotationSpeed)
    {
        if (rex::is_identical(rotationSpeed, m_orbit_camera.get_camera_rotation_speed()))
        {
            return;
        }

        m_orbit_camera.set_rotation_speed(rotationSpeed);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_pitch_range(const rex::MinMax<float> pitchRange)
    {
        auto pitch_range = m_orbit_camera.get_camera_pitch_range();

        if (rex::is_identical(pitchRange.minimum, pitch_range.minimum) && rex::is_identical(pitchRange.maximum, pitch_range.maximum))
        {
            return;
        }

        m_orbit_camera.set_pitch_range(pitchRange);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_mouse_move_sensitivity(float move)
    {
        if (rex::is_identical(move, m_orbit_camera.get_mouse_move_sensitivity()))
        {
            return;
        }

        m_orbit_camera.set_mouse_move_sensitivity(move);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_mouse_scroll_sensitivity(float scroll)
    {
        if (rex::is_identical(scroll, m_orbit_camera.get_mouse_scroll_sensitivity()))
        {
            return;
        }

        m_orbit_camera.set_mouse_scroll_sensitivity(scroll);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::set_viewport(float width, float height)
    {
        m_orbit_camera.set_viewport(width, height);
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::enable_yaw_rotation()
    {
        m_orbit_camera.enable_yaw_rotation();
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::enable_pitch_rotation()
    {
        m_orbit_camera.enable_pitch_rotation();
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::enable_zoom()
    {
        m_orbit_camera.enable_zoom();
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::disable_yaw_rotation()
    {
        m_orbit_camera.disable_yaw_rotation();
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::disable_pitch_rotation()
    {
        m_orbit_camera.disable_pitch_rotation();
    }
    //-------------------------------------------------------------------------
    void OrbitCameraController::disable_zoom()
    {
        m_orbit_camera.disable_zoom();
    }

    //-------------------------------------------------------------------------
    const rex::vec3& OrbitCameraController::get_focus() const
    {
        return m_orbit_camera.get_focus();
    }
    //-------------------------------------------------------------------------
    const float OrbitCameraController::get_focus_distance() const
    {
        return m_orbit_camera.get_focus_distance();
    }

    //-------------------------------------------------------------------------
    rex::Camera* OrbitCameraController::get_camera()
    {
        return m_orbit_camera.get_camera();
    }
    //-------------------------------------------------------------------------
    const rex::Camera* OrbitCameraController::get_camera() const
    {
        return m_orbit_camera.get_camera();
    }

    //-------------------------------------------------------------------------
    bool OrbitCameraController::is_enabled() const
    {
        return m_orbit_camera.is_enabled();
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::orbit()
    {
        if (m_orbit_camera.get_camera_transform() != nullptr)
        {
            // Look rotation
            rex::vec2 rotation_input = rex::vec2(m_horizontal_mouse_movement, m_vertical_mouse_movement);
            rex::quaternion rotation_default = m_orbit_camera.get_camera_transform()->get_rotation();

            rex::quaternion look_rotation = m_orbit_controller.calculate_look_rotation(rotation_input, rotation_default);

            // Look position
            rex::vec3 look_direction = look_rotation * rex::world_forward<float>();
            rex::vec3 look_position = m_focus_controller.calculate_focus_position(look_direction);

            m_orbit_camera.orbit(look_position, look_rotation);
        }
    }

    //-------------------------------------------------------------------------
    void OrbitCameraController::orbit(const rex::quaternion& lookRotation)
    {
        // Look position
        rex::vec3 look_direction = lookRotation * rex::world_forward<float>();
        rex::vec3 look_position = m_focus_controller.calculate_focus_position(look_direction);

        m_orbit_camera.orbit(look_position, lookRotation);
    }

    //-------------------------------------------------------------------------
    bool OrbitCameraController::on_mouse_scrolled(const rex::events::MouseScroll& scrollEvt)
    {
        if (!m_orbit_camera.can_zoom() || !is_enabled())
        {
            return false;
        }

        float current_distance = m_orbit_camera.get_focus_distance();

        float scroll_direction = scrollEvt.get_scroll_delta(rex::ScrollAxis::Y_AXIS);
        float scroll_sensitivity = m_orbit_camera.get_mouse_scroll_sensitivity();

        set_focus_distance(current_distance + ((scroll_direction * -scroll_sensitivity) * rex::World::get_delta_time().to_seconds()));

        return true;
    }

    //-------------------------------------------------------------------------
    bool OrbitCameraController::on_mouse_moved(const rex::events::MouseMove& moveEvt)
    {
        if (!m_orbit_camera.can_rotate() || !on_can_rotate() || !is_enabled())
        {
            return false;
        }

        float mouse_sensitivity = m_orbit_camera.get_mouse_move_sensitivity();

        m_horizontal_mouse_movement = m_orbit_camera.is_rotate_pitch_enabled() ? (moveEvt.get_mouse_delta().y * mouse_sensitivity) : 0.0f;
        m_vertical_mouse_movement = m_orbit_camera.is_rotate_yaw_enabled() ? (-moveEvt.get_mouse_delta().x * mouse_sensitivity) : 0.0f;

        return true;
    }
}