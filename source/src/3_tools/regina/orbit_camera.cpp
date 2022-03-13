#include "regina_pch.h"

#include "orbit_camera.h"

#include "camera.h"
#include "transform.h"

namespace regina
{
    //-------------------------------------------------------------------------
    OrbitCamera::OrbitCamera(const OrbitCameraDescription& desc)
        : m_description(desc)
        , m_camera(nullptr)
        , m_camera_transform(nullptr)
        , m_initial_camera_position(rex::vec3(0.0f))
        , m_initial_camera_rotation(rex::identity<rex::quaternion>())
        , m_camera_state({m_description.camera_settings.can_rotate_yaw, m_description.camera_settings.can_rotate_pitch, m_description.camera_settings.can_zoom})
    {
        if (!desc.camera_settings.can_rotate_pitch && !desc.camera_settings.can_rotate_yaw && !desc.camera_settings.can_zoom)
        {
            R_WARN("Camera movement is disabled!");
        }

        m_initial_camera_position = m_description.camera_settings.camera_position;
        m_initial_camera_rotation = m_description.camera_settings.camera_rotation;
        m_initial_camera_focus_distance = m_description.focus_settings.get_distance();

        reset();

        if (m_description.camera_settings.enabled)
        {
            enable();
        }
        else
        {
            disable();
        }
    }

    //-------------------------------------------------------------------------
    OrbitCamera::~OrbitCamera()
    {
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::enable()
    {
        if (m_description.camera_settings.enabled)
        {
            return;
        }

        m_description.camera_settings.can_rotate_yaw = m_camera_state.yaw_enabled;
        m_description.camera_settings.can_rotate_pitch = m_camera_state.pitch_enabled;
        m_description.camera_settings.can_zoom = m_camera_state.zoom_enabled;

        m_description.camera_settings.enabled = true;
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::disable()
    {
        if (!m_description.camera_settings.enabled)
        {
            return;
        }

        m_camera_state.yaw_enabled = m_description.camera_settings.can_rotate_yaw;
        m_camera_state.pitch_enabled = m_description.camera_settings.can_rotate_pitch;
        m_camera_state.zoom_enabled = m_description.camera_settings.can_zoom;

        disable_pitch_rotation();
        disable_yaw_rotation();
        disable_zoom();

        m_description.camera_settings.enabled = false;
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::reset()
    {
        if (m_camera_transform != nullptr)
        {
            m_camera_transform->set_position(m_initial_camera_position);
            m_camera_transform->set_rotation(m_initial_camera_rotation);
        }

        m_description.focus_settings.set_distance(m_initial_camera_focus_distance);
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_camera(rex::Camera* inCamera)
    {
        m_camera = inCamera;

        reset();
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_camera_transform(rex::Transform* inTransform)
    {
        m_camera_transform = inTransform;

        reset();
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_focus(const rex::vec3& target)
    {
        m_description.focus_settings.set_target(target);
    }
    //-------------------------------------------------------------------------
    void OrbitCamera::set_focus_distance(float distance)
    {
        m_description.focus_settings.set_distance(distance);
    }
    //-------------------------------------------------------------------------
    void OrbitCamera::set_focus_range(const rex::MinMax<float> distance)
    {
        m_description.focus_settings.set_min_distance(distance.minimum);
        m_description.focus_settings.set_max_distance(distance.maximum);
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_min_focus_distance(float minDistance)
    {
        m_description.focus_settings.set_min_distance(minDistance);
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_max_focus_distance(float maxDistance)
    {
        m_description.focus_settings.set_max_distance(maxDistance);
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_rotation_speed(float rotationSpeed)
    {
        m_description.orbit_settings.set_rotation_speed(rotationSpeed);
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_pitch_range(const rex::MinMax<float> pitchRange)
    {
        m_description.orbit_settings.set_minimum_pitch_angle(pitchRange.minimum);
        m_description.orbit_settings.set_maximum_pitch_angle(pitchRange.maximum);
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_mouse_move_sensitivity(float move)
    {
        m_description.mouse_settings.mouse_movement_sensitivity = move;
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_mouse_scroll_sensitivity(float scroll)
    {
        m_description.mouse_settings.mouse_scroll_sensitivity = scroll;
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::set_viewport(float width, float height)
    {
        if (m_camera != nullptr)
        {
            m_camera->set_viewport_size((int32)width, (int32)height);
        }
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::enable_yaw_rotation()
    {
        m_description.camera_settings.can_rotate_yaw = CanRotateYaw::YES;
    }
    //-------------------------------------------------------------------------
    void OrbitCamera::enable_pitch_rotation()
    {
        m_description.camera_settings.can_rotate_pitch = CanRotatePitch::YES;
    }
    //-------------------------------------------------------------------------
    void OrbitCamera::enable_zoom()
    {
        m_description.camera_settings.can_zoom = CanZoom::YES;
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::disable_yaw_rotation()
    {
        m_description.camera_settings.can_rotate_yaw = CanRotateYaw::NO;
    }
    //-------------------------------------------------------------------------
    void OrbitCamera::disable_pitch_rotation()
    {
        m_description.camera_settings.can_rotate_pitch = CanRotateYaw::NO;
    }
    //-------------------------------------------------------------------------
    void OrbitCamera::disable_zoom()
    {
        m_description.camera_settings.can_zoom = CanZoom::NO;
    }

    //-------------------------------------------------------------------------
    const rex::vec3& OrbitCamera::get_focus() const
    {
        return m_description.focus_settings.get_target();
    }
    //-------------------------------------------------------------------------
    const float OrbitCamera::get_focus_distance() const
    {
        return m_description.focus_settings.get_distance();
    }

    //-------------------------------------------------------------------------
    const float OrbitCamera::get_min_focus_distance() const
    {
        return m_description.focus_settings.get_min_distance();
    }

    //-------------------------------------------------------------------------
    const float OrbitCamera::get_max_focus_distance() const
    {
        return m_description.focus_settings.get_max_distance();
    }

    //-------------------------------------------------------------------------
    const float OrbitCamera::get_mouse_move_sensitivity() const
    {
        return m_description.mouse_settings.mouse_movement_sensitivity;
    }

    //-------------------------------------------------------------------------
    const float OrbitCamera::get_mouse_scroll_sensitivity() const
    {
        return m_description.mouse_settings.mouse_scroll_sensitivity;
    }

    //-------------------------------------------------------------------------
    const rex::quaternion& OrbitCamera::get_initial_camera_rotation() const
    {
        return m_initial_camera_rotation;
    }

    //-------------------------------------------------------------------------
    const float OrbitCamera::get_camera_rotation_speed() const
    {
        return m_description.orbit_settings.get_rotation_speed();
    }

    //-------------------------------------------------------------------------
    const rex::MinMax<float> OrbitCamera::get_camera_pitch_range() const
    {
        return {m_description.orbit_settings.get_minimum_pitch_angle(), m_description.orbit_settings.get_maximum_pitch_angle()};
    }

    //-------------------------------------------------------------------------
    rex::Camera* OrbitCamera::get_camera()
    {
        return m_camera;
    }
    //-------------------------------------------------------------------------
    const rex::Camera* OrbitCamera::get_camera() const
    {
        return m_camera;
    }

    //-------------------------------------------------------------------------
    rex::Transform* OrbitCamera::get_camera_transform()
    {
        return m_camera_transform;
    }

    //-------------------------------------------------------------------------
    const rex::Transform* OrbitCamera::get_camera_transform() const
    {
        return m_camera_transform;
    }

    //-------------------------------------------------------------------------
    const regina::FocusSettings& OrbitCamera::get_focus_settings() const
    {
        return m_description.focus_settings;
    }

    //-------------------------------------------------------------------------
    const regina::OrbitSettings& OrbitCamera::get_orbit_settings() const
    {
        return m_description.orbit_settings;
    }

    //-------------------------------------------------------------------------
    bool OrbitCamera::is_enabled() const
    {
        return m_description.camera_settings.enabled;
    }

    //-------------------------------------------------------------------------
    bool OrbitCamera::is_rotate_pitch_enabled() const
    {
        return m_description.camera_settings.can_rotate_pitch;
    }

    //-------------------------------------------------------------------------
    bool OrbitCamera::is_rotate_yaw_enabled() const
    {
        return m_description.camera_settings.can_rotate_yaw;
    }

    //-------------------------------------------------------------------------
    void OrbitCamera::orbit(const rex::vec3& lookPosition, const rex::quaternion& lookRotation)
    {
        if (m_camera_transform != nullptr)
        {
            m_camera_transform->set_position(lookPosition);
            m_camera_transform->set_rotation(lookRotation);
        }
    }

    //-------------------------------------------------------------------------
    bool OrbitCamera::can_rotate() const
    {
        bool can_rotate_yaw = m_description.camera_settings.can_rotate_yaw;
        bool can_rotate_pitch = m_description.camera_settings.can_rotate_pitch;

        if ((!can_rotate_yaw && !can_rotate_pitch))
        {
            return false;
        }

        return true;
    }

    //-------------------------------------------------------------------------
    bool OrbitCamera::can_zoom() const
    {
        return m_description.camera_settings.can_zoom;
    }
}