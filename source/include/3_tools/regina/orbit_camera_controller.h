#pragma once

#include "world.h"
#include "event.h"
#include "min_max.h"
#include "orbit_camera.h"

namespace rex
{
    struct FrameInfo;

    class Transform;
    class Camera;

    namespace events
    {
        class MouseMove;
        class MouseScroll;
    }
}

namespace regina
{
    class FocusController
    {
    public:
        //-------------------------------------------------------------------------
        FocusController(const FocusSettings& settings)
            :m_focus_settings(settings)
        {}
        ~FocusController() = default;

        //-------------------------------------------------------------------------
        void reset(const FocusSettings& settings)
        {
            m_focus_settings = settings;
        }

        //-------------------------------------------------------------------------
        rex::vec3 calculate_focus_position(const rex::vec3& lookDirection)
        {
            rex::vec3 look_direction = rex::normalize(lookDirection);
            rex::vec3 look_position = m_focus_settings.get_target() - (look_direction * m_focus_settings.get_distance());

            return look_position;
        }

    private:
        FocusSettings m_focus_settings;
    };
    class OrbitController
    {
    public:
        //-------------------------------------------------------------------------
        OrbitController(const rex::vec3& orbitAngles, const OrbitSettings& orbitSettings)
            :m_orbit_angles(orbitAngles)
            ,m_orbit_settings(orbitSettings)
        {
        }
        ~OrbitController() = default;

        //-------------------------------------------------------------------------
        void reset(const rex::vec3& orbitAngles, const OrbitSettings& orbitSettings)
        {
            m_orbit_angles = orbitAngles;
            m_orbit_settings = orbitSettings;
        }

        //-------------------------------------------------------------------------
        rex::quaternion calculate_look_rotation(const rex::vec2& rotationInput, const rex::quaternion& defaultRotation)
        {
            rex::vec3 delta = orbit(rotationInput, m_orbit_settings);

            rex::quaternion look_rotation = rex::identity<rex::quaternion>();
            if (rex::is_identical(delta, rex::vec3(0.0f, 0.0f, 0.0f)))
            {
                look_rotation = defaultRotation;
            }
            else
            {
                m_orbit_angles = m_orbit_angles + delta;
                m_orbit_angles = constrain(m_orbit_angles, m_orbit_settings);

                look_rotation = rex::quaternion(rex::vec3(m_orbit_angles.x, m_orbit_angles.y, m_orbit_angles.z));
            }

            return look_rotation;
        }

    private:
        //-------------------------------------------------------------------------
        rex::vec3 orbit(const rex::vec2& rotationInput, const OrbitSettings& settings)
        {
            const float e = 0.001f;
            if (rotationInput.x < -e || rotationInput.x > e || rotationInput.y < -e || rotationInput.y > e)
            {
                rex::vec2 orbit = settings.get_rotation_speed() * rex::World::get_delta_time().to_seconds() * rotationInput;

                return rex::vec3(orbit, 0.0f);
            }

            return rex::vec3(0.0f, 0.0f, 0.0f);
        }
        //-------------------------------------------------------------------------
        rex::vec3 constrain(const rex::vec3& orbitAngles, const OrbitSettings& settings)
        {
            rex::vec3 new_orbit_angles = orbitAngles;

            if (new_orbit_angles.x > rex::pi<float>() / 2.0f)
            {
                new_orbit_angles.x -= rex::pi<float>();
                new_orbit_angles.x = rex::clamp(new_orbit_angles.x, rex::deg2rad(settings.get_minimum_pitch_angle()), rex::deg2rad(settings.get_maximum_pitch_angle()));
                new_orbit_angles.x += rex::pi<float>();
            }
            else if (new_orbit_angles.x < -(rex::pi<float>() / 2.0f))
            {
                new_orbit_angles.x += rex::pi<float>();
                new_orbit_angles.x = rex::clamp(new_orbit_angles.x, rex::deg2rad(settings.get_minimum_pitch_angle()), rex::deg2rad(settings.get_maximum_pitch_angle()));
                new_orbit_angles.x -= rex::pi<float>();
            }
            else
            {
                new_orbit_angles.x = rex::clamp(new_orbit_angles.x, rex::deg2rad(settings.get_minimum_pitch_angle()), rex::deg2rad(settings.get_maximum_pitch_angle()));
            }

            if (new_orbit_angles.y < -(rex::pi<float>() / 2.0f))
            {
                new_orbit_angles.y += rex::pi<float>() * 2;
            }
            else if (new_orbit_angles.y > (rex::pi<float>() / 2.0f))
            {
                new_orbit_angles.y -= rex::pi<float>() * 2;
            }

            new_orbit_angles.z = orbitAngles.z;

            return new_orbit_angles;
        }

        rex::vec3       m_orbit_angles;
        OrbitSettings   m_orbit_settings;
    };

    class OrbitCameraController
    {
    public:
        OrbitCameraController(const OrbitCameraDescription& desc);
        virtual ~OrbitCameraController();

        void                                enable();
        void                                disable();

        void                                reset();
             
        void                                on_update(const rex::FrameInfo& info);
        void                                on_event(rex::events::Event& evt);

        void                                set_camera(rex::Camera* inCamera);
        void                                set_camera_transform(rex::Transform* inTransform);
        void                                set_viewport(float width, float height);

        void                                set_focus(const rex::vec3& target);
        void                                set_focus_distance(float distance);
        void                                set_focus_range(const rex::MinMax<float> distance);

        void                                set_rotation_speed(float rotationSpeed);
        void                                set_pitch_range(const rex::MinMax<float> pitchRange);

        void                                set_mouse_move_sensitivity(float move);
        void                                set_mouse_scroll_sensitivity(float scroll);

        void                                enable_yaw_rotation();
        void                                enable_pitch_rotation();
        void                                enable_zoom();
            
        void                                disable_yaw_rotation();
        void                                disable_pitch_rotation();
        void                                disable_zoom();

        const rex::vec3&                 get_focus() const;
        const float                         get_focus_distance() const;

        rex::Camera*                             get_camera();
        const rex::Camera*                       get_camera() const;

        bool                                is_enabled() const;

    protected:
        virtual bool                        on_can_rotate() const { return true; }

    private:
        void                                orbit();
        void                                orbit(const rex::quaternion& lookRotation);

        bool                                on_mouse_scrolled(const rex::events::MouseScroll& scrollEvt);
        bool                                on_mouse_moved(const rex::events::MouseMove& moveEvt);

        FocusController                     m_focus_controller;
        OrbitController                     m_orbit_controller;

        OrbitCamera                         m_orbit_camera;

        float                               m_vertical_mouse_movement;
        float                               m_horizontal_mouse_movement;
    };
}