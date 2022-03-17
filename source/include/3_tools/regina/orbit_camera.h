#pragma once

#include "yes_no_enum.h"
#include "min_max.h"
#include "algorithms/clamp.h"
#include "aspect_ratio.h"
#include "field_of_view.h"
#include "clipping_planes.h"

namespace rex
{
    class Camera;
    class Transform;
}

namespace regina
{
    using CanRotateYaw = rex::YesNoEnum;
    using CanRotatePitch = rex::YesNoEnum;
    using CanZoom = rex::YesNoEnum;

    class FocusSettings
    {
    public:
        FocusSettings()
            : m_target({0.0f, 0.0f, 0.0f})
            , m_distance(5.0f)
            , m_min_distance(1.0f)
            , m_max_distance(20.0f)
        {
        }

        //-------------------------------------------------------------------------
        void set_target(const rex::vec3& target)
        {
            m_target = target;
        }

        //-------------------------------------------------------------------------
        void set_distance(float distance)
        {
            m_distance = rex::clamp(distance, m_min_distance, m_max_distance);
        }
        //-------------------------------------------------------------------------
        void set_min_distance(float distance)
        {
            m_min_distance = distance;
        }
        //-------------------------------------------------------------------------
        void set_max_distance(float distance)
        {
            m_max_distance = distance;
        }

        //-------------------------------------------------------------------------
        const rex::vec3& get_target() const
        {
            return m_target;
        }

        //-------------------------------------------------------------------------
        float get_distance() const
        {
            return m_distance;
        }
        //-------------------------------------------------------------------------
        float get_min_distance() const
        {
            return m_min_distance;
        }
        //-------------------------------------------------------------------------
        float get_max_distance() const
        {
            return m_max_distance;
        }

    private:
        rex::vec3 m_target;

        float m_distance;
        float m_min_distance;
        float m_max_distance;
    };
    class OrbitSettings
    {
    public:
        OrbitSettings()
            : m_rotation_speed(90.0f)
            , m_min_pitch_angle(-30.0f)
            , m_max_pitch_angle(60.0f)
        {
        }

        //-------------------------------------------------------------------------
        void set_rotation_speed(float speed)
        {
            m_rotation_speed = speed;
        }
        //-------------------------------------------------------------------------
        void set_minimum_pitch_angle(float angle)
        {
            m_min_pitch_angle = angle;

            if (m_max_pitch_angle < m_min_pitch_angle)
            {
                m_max_pitch_angle = m_min_pitch_angle;
            }
        }
        //-------------------------------------------------------------------------
        void set_maximum_pitch_angle(float angle)
        {
            m_max_pitch_angle = angle;

            if (m_max_pitch_angle < m_min_pitch_angle)
            {
                m_max_pitch_angle = m_min_pitch_angle;
            }
        }

        //-------------------------------------------------------------------------
        float get_rotation_speed() const
        {
            return m_rotation_speed;
        }
        //-------------------------------------------------------------------------
        float get_minimum_pitch_angle() const
        {
            return m_min_pitch_angle;
        }
        //-------------------------------------------------------------------------
        float get_maximum_pitch_angle() const
        {
            return m_max_pitch_angle;
        }

    private:
        float m_rotation_speed;
        float m_min_pitch_angle;
        float m_max_pitch_angle;
    };

    struct MouseSettings
    {
        MouseSettings()
            : mouse_movement_sensitivity(0.1f)
            , mouse_scroll_sensitivity(200.0f)
        {
        }

        float mouse_movement_sensitivity;
        float mouse_scroll_sensitivity;
    };
    struct CameraSettings
    {
        CameraSettings()
            : camera_position({0.0f, 2.0f, 4.0f})
            , camera_rotation(rex::identity<rex::quaternion>())
            , can_rotate_yaw(CanRotateYaw::YES)
            , can_rotate_pitch(CanRotatePitch::YES)
            , can_zoom(CanZoom::YES)
            , enabled(true)
        {
        }

        rex::vec3 camera_position;
        rex::quaternion camera_rotation;

        CanRotateYaw can_rotate_yaw;
        CanRotatePitch can_rotate_pitch;
        CanZoom can_zoom;

        bool enabled;
    };

    struct OrbitCameraDescription
    {
        OrbitCameraDescription()
            : camera_settings()
            , focus_settings()
            , orbit_settings()
            , mouse_settings()
        {
        }

        CameraSettings camera_settings;
        FocusSettings focus_settings;
        OrbitSettings orbit_settings;
        MouseSettings mouse_settings;
    };

    class OrbitCamera
    {
    public:
        OrbitCamera(const OrbitCameraDescription& desc);
        virtual ~OrbitCamera();

        void enable();
        void disable();

        void reset();

        void set_camera(rex::Camera* inCamera);
        void set_camera_transform(rex::Transform* inTransform);
        void set_viewport(float width, float height);

        void set_focus(const rex::vec3& target);
        void set_focus_distance(float distance);
        void set_focus_range(const rex::MinMax<float> distance);
        void set_min_focus_distance(float minDistance);
        void set_max_focus_distance(float maxDistance);

        void set_rotation_speed(float rotationSpeed);
        void set_pitch_range(const rex::MinMax<float> pitchRange);

        void set_mouse_move_sensitivity(float move);
        void set_mouse_scroll_sensitivity(float scroll);

        void enable_yaw_rotation();
        void enable_pitch_rotation();
        void enable_zoom();

        void disable_yaw_rotation();
        void disable_pitch_rotation();
        void disable_zoom();

        const rex::vec3& get_focus() const;
        const float get_focus_distance() const;
        const float get_min_focus_distance() const;
        const float get_max_focus_distance() const;

        const float get_mouse_move_sensitivity() const;
        const float get_mouse_scroll_sensitivity() const;

        const rex::quaternion& get_initial_camera_rotation() const;
        const float get_camera_rotation_speed() const;
        const rex::MinMax<float> get_camera_pitch_range() const;

        rex::Camera* get_camera();
        const rex::Camera* get_camera() const;

        rex::Transform* get_camera_transform();
        const rex::Transform* get_camera_transform() const;

        const FocusSettings& get_focus_settings() const;
        const OrbitSettings& get_orbit_settings() const;

        bool is_enabled() const;
        bool is_rotate_pitch_enabled() const;
        bool is_rotate_yaw_enabled() const;

        bool can_rotate() const;
        bool can_zoom() const;

        void orbit(const rex::vec3& lookPosition, const rex::quaternion& lookRotation);

    private:
        struct CameraState
        {
            CanRotateYaw yaw_enabled;
            CanRotatePitch pitch_enabled;
            CanZoom zoom_enabled;
        };

        OrbitCameraDescription m_description;

        rex::Transform* m_camera_transform;
        rex::Camera* m_camera;

        rex::vec3 m_initial_camera_position;
        rex::quaternion m_initial_camera_rotation;
        float m_initial_camera_focus_distance;

        CameraState m_camera_state;
    };
}