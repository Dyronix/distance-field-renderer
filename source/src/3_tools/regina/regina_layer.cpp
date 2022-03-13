#include "regina_pch.h"

#include "regina_layer.h"

#include "ecs/scene.h"
#include "ecs/entity.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/tag_component.h"
#include "ecs/components/model_component.h"
#include "ecs/components/material_component.h"
#include "ecs/components/camera_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/directional_light_component.h"

#include "renderpasses/clear_pass.h"
#include "renderpasses/blur_pass.h"
#include "renderpasses/composite_pass.h"
#include "renderpasses/geometry_pass.h"
#include "renderpasses/pre_depth_pass.h"

#include "scene_renderer.h"

#include "core_window.h"

#include "aspect_ratio.h"
#include "field_of_view.h"

#include "input/win32_input.h"
#include "input/mouse/win32_mouse_codes.h"

namespace regina
{
    // Render pass settings
    const rex::StringID PREDEPTHPASS_NAME = "PreDepthPass"_sid;
    const rex::StringID GEOMETRYPASS_NAME = "GeometryPass"_sid;
    const rex::StringID COMPOSITEPASS_NAME = "CompositePass"_sid;

    namespace camera_settings
    {
        float NEAR_PLANE = 0.01f;
        float FAR_PLANE = 1000.0f;

        float FIELD_OF_VIEW = 95.142f;

        rex::vec3 CAMERA_POSITION = {2.4f, 1.3f, 0.17f};
        rex::vec3 CAMERA_FOCUS = {0.0f, 1.0f, 0.0f};
        rex::vec3 CAMERA_ROTATION = {3.0f, 1.5f, 0.0f};

        bool CAN_ROTATE_PITCH = true;
        bool CAN_ROTATE_YAW = true;
        bool CAN_ZOOM = true;
        bool IS_ENABLED = true;

        float MIN_FOCUS_DISTANCE = 1.0f;
        float MAX_FOCUS_DISTANCE = 10.0f;
        float FOCUS_DISTANCE = 2.5f;

        float ROTATION_SPEED = 0.25f;
        float MIN_PITCH_ANGLE = -50.0f;
        float MAX_PITCH_ANGLE = 50.0f;

        float MOVE_SENSITIVITY = 1.0f;
        float SCROLL_SENSITIVITY = 2.0f;
    }

    namespace renderpass_settings
    {
        bool BACKFACE_CULLING = true;
        bool SHOW_ENVIRONMENT = true;
        bool SHOW_GRID = true;
        float GAMMA_CORRECTION = 2.55f;
    }

    //-------------------------------------------------------------------------
    rex::PreDepthPassOptions create_pre_depth_pass_options()
    {
        rex::PreDepthPassOptions options;

        options.pass_name = PREDEPTHPASS_NAME;
        options.shader_name = "predepth"_sid;
        options.near_plane = camera_settings::NEAR_PLANE;
        options.far_plane = camera_settings::FAR_PLANE;
        options.backface_culling = renderpass_settings::BACKFACE_CULLING;

        return options;
    }
    //-------------------------------------------------------------------------
    rex::GeometryPassOptions create_geometry_pass_options()
    {
        rex::GeometryPassOptions options;

        options.pass_name = GEOMETRYPASS_NAME;
        options.show_environment = renderpass_settings::SHOW_ENVIRONMENT;
        options.show_grid = renderpass_settings::SHOW_GRID;
        options.backface_culling = renderpass_settings::BACKFACE_CULLING;
        options.geometry_shader_name = "pbr_static"_sid;
        options.skybox_shader_name = "skybox"_sid;

        return options;
    }
    //-------------------------------------------------------------------------
    rex::CompositePassOptions create_composite_pass_options()
    {
        rex::CompositePassOptions options;

        options.pass_name = COMPOSITEPASS_NAME;
        options.shader_name = "blit"_sid;
        options.color_buffer = GEOMETRYPASS_NAME;
        options.gamma_correction = renderpass_settings::GAMMA_CORRECTION;

        return options;
    }

    //-------------------------------------------------------------------------
    regina::FocusSettings create_focus_settings(const rex::vec3& target, const float minFocusDistance, float maxFocusDistance, float focusDistance)
    {
        regina::FocusSettings settings;

        settings.set_target(target);
        settings.set_min_distance(minFocusDistance);
        settings.set_max_distance(maxFocusDistance);
        settings.set_distance(focusDistance);

        return settings;
    }
    //-------------------------------------------------------------------------
    OrbitSettings create_orbit_settings(const float rotationSpeed, const float minPitchAngle, const float maxPitchAngle)
    {
        OrbitSettings settings;

        settings.set_rotation_speed(rotationSpeed);
        settings.set_minimum_pitch_angle(minPitchAngle);
        settings.set_maximum_pitch_angle(maxPitchAngle);

        return settings;
    }
    //-------------------------------------------------------------------------
    MouseSettings create_mouse_settings(const float moveSensitivity, const float scrollSensitivity)
    {
        MouseSettings settings;

        settings.mouse_movement_sensitivity = moveSensitivity;
        settings.mouse_scroll_sensitivity = scrollSensitivity;

        return settings;
    }
    //-------------------------------------------------------------------------
    OrbitCameraDescription create_orbit_camera_description()
    {
        OrbitCameraDescription description;

        // Camera
        rex::vec3 camera_pos = camera_settings::CAMERA_POSITION;
        rex::quaternion camera_rot = rex::quaternion(camera_settings::CAMERA_ROTATION);

        bool can_rotate_pitch = camera_settings::CAN_ROTATE_PITCH;
        bool can_rotate_yaw = camera_settings::CAN_ROTATE_YAW;
        bool can_zoom = camera_settings::CAN_ZOOM;

        bool initial_enabled = camera_settings::IS_ENABLED;

        description.camera_settings.camera_position = camera_pos;
        description.camera_settings.camera_rotation = camera_rot;
        description.camera_settings.can_rotate_pitch = can_rotate_pitch ? CanRotatePitch::YES : CanRotatePitch::NO;
        description.camera_settings.can_rotate_yaw = can_rotate_yaw ? CanRotateYaw::YES : CanRotateYaw::NO;
        description.camera_settings.can_zoom = can_zoom ? CanZoom::YES : CanZoom::NO;
        description.camera_settings.enabled = initial_enabled;

        // Focus
        description.focus_settings = create_focus_settings(camera_settings::CAMERA_FOCUS, camera_settings::MIN_FOCUS_DISTANCE, camera_settings::MAX_FOCUS_DISTANCE, camera_settings::FOCUS_DISTANCE);

        // Orbit
        description.orbit_settings = create_orbit_settings(camera_settings::ROTATION_SPEED, camera_settings::MIN_PITCH_ANGLE, camera_settings::MAX_PITCH_ANGLE);

        // Mouse
        description.mouse_settings = create_mouse_settings(camera_settings::MOVE_SENSITIVITY, camera_settings::SCROLL_SENSITIVITY);

        return description;
    }

    //-------------------------------------------------------------------------
    ReginaLayer::ReginaLayer(const rex::CoreWindow* window)
        : Layer("regina_layer"_sid, 0, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, create_orbit_camera_description())
        , m_window(window)
    {
    }
    //-------------------------------------------------------------------------
    ReginaLayer::~ReginaLayer()
    {
    }

    //-------------------------------------------------------------------------
    void ReginaLayer::on_attach()
    {
        // load shader, deferred shader: g-buffer
        // load shader, deferred shader: light
        // load shader, deferred shader: light-box

        // load model, lattice

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void ReginaLayer::on_detach()
    {
        m_scene_renderer->destroy();
        m_scene_renderer.reset();

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void ReginaLayer::on_update(const rex::FrameInfo& info)
    {
        m_camera_controller.on_update(info);
    }

    //-------------------------------------------------------------------------
    void ReginaLayer::on_event(rex::events::Event& event)
    {
        m_camera_controller.on_event(event);

        m_scene_renderer->set_viewport_width(m_window->get_width());
        m_scene_renderer->set_viewport_height(m_window->get_height());
        m_scene_renderer->begin_scene();
        m_scene_renderer->end_scene();
    }

    //-------------------------------------------------------------------------
    void ReginaLayer::setup_scene()
    {
        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        rex::ecs::Entity light_00 = m_scene->create_entity("light_00"_sid);
        rex::ecs::Entity light_01 = m_scene->create_entity("light_01"_sid);
        rex::ecs::Entity light_02 = m_scene->create_entity("light_02"_sid);
        rex::ecs::Entity light_03 = m_scene->create_entity("light_03"_sid);
        rex::ecs::Entity light_04 = m_scene->create_entity("light_04"_sid);
        rex::ecs::Entity light_05 = m_scene->create_entity("light_05"_sid);

        light_00.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(+20.0f, +130.0f, +35.0f));
        light_00.add_component<rex::ecs::PointLightComponent>(1.0f, 100.0f, 100.0f);
        light_01.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(+50.0f, -20.0f, +20.0f));
        light_01.add_component<rex::ecs::PointLightComponent>(1.0f, 1000.0f, 1000.0f);
        light_02.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(-50.0f, -80.0f, -50.0f));
        light_02.add_component<rex::ecs::PointLightComponent>(1.0f, 1000.0f, 1000.0f);
        light_03.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(-130.0f, +60.0f, -100.0f));
        light_03.add_component<rex::ecs::PointLightComponent>(1.0f, 800.0f, 800.0f);
        light_04.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(+100.0f, +60.0f, -150.0f));
        light_04.add_component<rex::ecs::PointLightComponent>(1.0f, 5000.0f, 5000.0f);
        light_05.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(-100.0f, +60.0f, +150.0f));
        light_05.add_component<rex::ecs::PointLightComponent>(1.0f, 5000.0f, 5000.0f);

        rex::ecs::Entity dirlight = m_scene->create_entity("directional_light"_sid);

        dirlight.add_component<rex::ecs::DirectionalLightComponent>();
    }
    //-------------------------------------------------------------------------
    void ReginaLayer::setup_camera()
    {
        float viewport_width = (float)m_window->get_width();
        float viewport_height = (float)m_window->get_height();

        rex::AspectRatio aspect_ratio = rex::AspectRatio(viewport_width, viewport_height);

        float near_plane = camera_settings::NEAR_PLANE;
        float far_plane = camera_settings::FAR_PLANE;
        float fov = camera_settings::FIELD_OF_VIEW;

        m_camera.set_perspective(rex::FieldOfView(rex::DegAngle(fov), aspect_ratio.get_ratio()), rex::ClippingPlanes(near_plane, far_plane));
        m_camera.activate();

        rex::ecs::Entity camera_entity = m_scene->create_entity("camera"_sid);

        rex::ecs::TransformComponent& transform_component = camera_entity.get_component<rex::ecs::TransformComponent>();
        rex::ecs::CameraComponent& camera_component = camera_entity.add_component<rex::ecs::CameraComponent>(m_camera);

        m_camera_controller.set_camera(&camera_component.camera);
        m_camera_controller.set_camera_transform(&transform_component.transform);
    }
    //-------------------------------------------------------------------------
    void ReginaLayer::setup_scene_renderer()
    {
        rex::SceneRenderPasses renderpasses;

        // auto predepth = create_pre_depth_pass(create_pre_depth_pass_options());
        // auto geometry = create_geometry_pass(create_geometry_pass_options());
        // auto composite = create_composite_pass(create_composite_pass_options());
        //
        // renderpasses.push_back(std::move(predepth));
        // renderpasses.push_back(std::move(geometry));
        // renderpasses.push_back(std::move(composite));

        m_scene_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(renderpasses));
    }

    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> ReginaLayer::create_pre_depth_pass(const rex::PreDepthPassOptions& options) const
    {
        return std::make_unique<rex::PreDepthPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> ReginaLayer::create_geometry_pass(const rex::GeometryPassOptions& options) const
    {
        return std::make_unique<rex::GeometryPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> ReginaLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
}