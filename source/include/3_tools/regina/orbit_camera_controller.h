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
        FocusController(const OrbitCamera& camera);
        ~FocusController();

        rex::vec3 calculate_focus_position(const rex::vec3& lookDirection);

    private:
        const FocusSettings& m_focus_settings;
    };

    class OrbitController
    {
    public:
        OrbitController(const rex::vec3& orbitAngles, const OrbitCamera& camera);
        ~OrbitController();

        void reset_orbit_angles(const rex::vec3& orbitAngles);

        rex::quaternion calculate_look_rotation(const rex::vec2& rotationInput, const rex::quaternion& defaultRotation);

    private:
        rex::vec3 orbit(const rex::vec2& rotationInput, const OrbitSettings& settings);
        rex::vec3 constrain(const rex::vec3& orbitAngles, const OrbitSettings& settings);

        rex::vec3 m_orbit_angles;
        const OrbitSettings& m_orbit_settings;
    };

    class OrbitCameraController
    {
    public:
        OrbitCameraController(const OrbitCameraDescription& desc);
        virtual ~OrbitCameraController();

        void enable();
        void disable();

        void reset();
        void orbit(const glm::vec2& orbitDelta);

        void on_update(const rex::FrameInfo& info);
        void on_event(rex::events::Event& evt);

        void set_camera(rex::Camera* inCamera);
        void set_camera_transform(rex::Transform* inTransform);
        void set_viewport(float width, float height);

        void set_focus(const rex::vec3& target);
        void set_focus_distance(float distance);
        void set_focus_range(const rex::MinMax<float> distance);

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

        rex::Camera* get_camera();
        const rex::Camera* get_camera() const;

        rex::Transform* get_camera_transform();
        const rex::Transform* get_camera_transform() const;

        bool is_enabled() const;

    protected:
        virtual bool on_can_rotate() const
        {
            return true;
        }

    private:
        void orbit();
        void orbit(const rex::quaternion& lookRotation);

        bool on_mouse_scrolled(const rex::events::MouseScroll& scrollEvt);
        bool on_mouse_moved(const rex::events::MouseMove& moveEvt);

        std::unique_ptr<FocusController> m_focus_controller;
        std::unique_ptr<OrbitController> m_orbit_controller;

        OrbitCamera m_orbit_camera;

        float m_vertical_mouse_movement;
        float m_horizontal_mouse_movement;
    };
}