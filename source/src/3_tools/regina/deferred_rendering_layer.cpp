#include "regina_pch.h"

#include "deferred_rendering_layer.h"

#include "ecs/components/camera_component.h"
#include "ecs/components/directional_light_component.h"
#include "ecs/components/material_component.h"
#include "ecs/components/model_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/tag_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/entity.h"
#include "ecs/scene.h"

#include "renderpasses/blur_pass.h"
#include "renderpasses/clear_pass.h"
#include "renderpasses/composite_pass.h"
#include "renderpasses/deferred_geometry_pass.h"
#include "renderpasses/deferred_light_pass.h"
#include "renderpasses/pre_depth_pass.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/frame_buffer_pool.h"
#include "resources/uniform_buffer_set.h"

#include "mesh_factory.h"

#include "scene_renderer.h"

#include "core_window.h"

#include "aspect_ratio.h"
#include "field_of_view.h"

#include "input/mouse/win32_mouse_codes.h"
#include "input/win32_input.h"

#include "graphics/resources/gl_shader.h"
#include "graphics/resources/gl_shader_program.h"

#include "file_import.h"
#include "file_memory.h"

namespace regina
{
    // Render pass settings
    namespace deferred_rendering
    {
        const rex::StringID PREDEPTHPASS_NAME = "PreDepthPass"_sid;
        const rex::StringID DEFERREDGEOMETRYPASS_NAME = "DeferredGeometryPass"_sid;
        const rex::StringID DEFERREDLIGHTPASS_NAME = "DeferredLightPass"_sid;
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
        } // namespace camera_settings

        namespace renderpass_settings
        {
            bool BACKFACE_CULLING = true;
            bool SHOW_ENVIRONMENT = false;
            bool SHOW_GRID = true;
            float GAMMA_CORRECTION = 2.55f;
        } // namespace renderpass_settings

        //-------------------------------------------------------------------------
        rex::PreDepthPassOptions create_pre_depth_pass_options()
        {
            rex::PreDepthPassOptions options;

            options.pass_name = deferred_rendering::PREDEPTHPASS_NAME;
            options.shader_name = "predepth"_sid;
            options.near_plane = deferred_rendering::camera_settings::NEAR_PLANE;
            options.far_plane = deferred_rendering::camera_settings::FAR_PLANE;
            options.backface_culling = deferred_rendering::renderpass_settings::BACKFACE_CULLING;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredGeometryPassOptions create_deferred_geometry_pass_options()
        {
            rex::DeferredGeometryPassOptions options;

            options.pass_name = deferred_rendering::DEFERREDGEOMETRYPASS_NAME;
            options.backface_culling = deferred_rendering::renderpass_settings::BACKFACE_CULLING;
            options.geometry_shader_name = "g_buffer"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightPassOptions create_deferred_light_pass_options()
        {
            rex::DeferredLightPassOptions options;

            options.pass_name = deferred_rendering::DEFERREDLIGHTPASS_NAME;
            options.shader_name = "deferred_shading_lighting"_sid;
            options.g_position_buffer = {deferred_rendering::DEFERREDGEOMETRYPASS_NAME, 0};
            options.g_normal_buffer = {deferred_rendering::DEFERREDGEOMETRYPASS_NAME, 1};
            options.g_albedo_spec_buffer = {deferred_rendering::DEFERREDGEOMETRYPASS_NAME, 2};

            return options;
        }
        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options()
        {
            rex::CompositePassOptions options;

            options.pass_name = deferred_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = deferred_rendering::DEFERREDLIGHTPASS_NAME;
            options.gamma_correction = deferred_rendering::renderpass_settings::GAMMA_CORRECTION;

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
            rex::vec3 camera_pos = deferred_rendering::camera_settings::CAMERA_POSITION;
            rex::quaternion camera_rot = rex::quaternion(deferred_rendering::camera_settings::CAMERA_ROTATION);

            bool can_rotate_pitch = deferred_rendering::camera_settings::CAN_ROTATE_PITCH;
            bool can_rotate_yaw = deferred_rendering::camera_settings::CAN_ROTATE_YAW;
            bool can_zoom = deferred_rendering::camera_settings::CAN_ZOOM;

            bool initial_enabled = deferred_rendering::camera_settings::IS_ENABLED;

            description.camera_settings.camera_position = camera_pos;
            description.camera_settings.camera_rotation = camera_rot;
            description.camera_settings.can_rotate_pitch = can_rotate_pitch ? CanRotatePitch::YES : CanRotatePitch::NO;
            description.camera_settings.can_rotate_yaw = can_rotate_yaw ? CanRotateYaw::YES : CanRotateYaw::NO;
            description.camera_settings.can_zoom = can_zoom ? CanZoom::YES : CanZoom::NO;
            description.camera_settings.enabled = initial_enabled;

            // Focus
            description.focus_settings = create_focus_settings(deferred_rendering::camera_settings::CAMERA_FOCUS, deferred_rendering::camera_settings::MIN_FOCUS_DISTANCE, deferred_rendering::camera_settings::MAX_FOCUS_DISTANCE,
                                                               deferred_rendering::camera_settings::FOCUS_DISTANCE);

            // Orbit
            description.orbit_settings = create_orbit_settings(deferred_rendering::camera_settings::ROTATION_SPEED, deferred_rendering::camera_settings::MIN_PITCH_ANGLE, deferred_rendering::camera_settings::MAX_PITCH_ANGLE);

            // Mouse
            description.mouse_settings = create_mouse_settings(deferred_rendering::camera_settings::MOVE_SENSITIVITY, deferred_rendering::camera_settings::SCROLL_SENSITIVITY);

            return description;
        }

        //-------------------------------------------------------------------------
        void load_shader(const rex::StringID& name, const rex::StringID& queue, const rex::StringID& vertexCodePath, const rex::StringID& fragmentCodePath)
        {
            rex::ShaderProgramCreationInfo creation_info;

            creation_info.tag = name;
            creation_info.queue = queue;
            creation_info.vertexShaderCode = rex::read_from_disk(vertexCodePath.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);
            creation_info.fragmentShaderCode = rex::read_from_disk(fragmentCodePath.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);

            if (creation_info.vertexShaderCode && creation_info.fragmentShaderCode)
            {
                rex::ref_ptr<rex::ShaderProgram> program = rex::ResourceFactory::create_shader_program(std::move(creation_info));
                rex::shader_library::add(program);
            }
            else
            {
                R_WARN("{0} shader was not found on disk", creation_info.tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void load_shaders()
        {
            load_shader("blit"_sid, "1000"_sid, "assets\\blit.vertex"_sid, "assets\\blit.fragment"_sid);
            load_shader("predepth"_sid, "1000"_sid, "assets\\predepth.vertex"_sid, "assets\\predepth.fragment"_sid);
            load_shader("g_buffer"_sid, "1000"_sid, "assets\\g_buffer.vertex"_sid, "assets\\g_buffer.fragment"_sid);
            load_shader("deferred_shading_lighting"_sid, "1000"_sid, "assets\\deferred_shading_lighting.vertex"_sid, "assets\\deferred_shading_lighting.fragment"_sid);
        }
        //-------------------------------------------------------------------------
        void load_primitive_geometry()
        {
            rex::mesh_factory::load();
        }
        //-------------------------------------------------------------------------
        void load_custom_geometry()
        {

        }
    } // namespace deferred_rendering

    //-------------------------------------------------------------------------
    DeferredRenderingLayer::DeferredRenderingLayer(const rex::CoreWindow* window)
        : Layer("regina_layer"_sid, 0, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, deferred_rendering::create_orbit_camera_description())
        , m_window(window)
    {
    }
    //-------------------------------------------------------------------------
    DeferredRenderingLayer::~DeferredRenderingLayer()
    {
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::on_attach()
    {
        deferred_rendering::load_shaders();
        deferred_rendering::load_primitive_geometry();
        deferred_rendering::load_custom_geometry();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::on_detach()
    {
        rex::shader_library::clear();
        rex::mesh_factory::clear();

        rex::FrameBufferPool::instance()->clear();
        rex::UniformBufferSet::instance()->clear();

        m_scene_renderer->destroy();
        m_scene_renderer.reset();

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::on_update(const rex::FrameInfo& info)
    {
        m_camera_controller.on_update(info);

        m_scene_renderer->set_viewport_width(m_window->get_width());
        m_scene_renderer->set_viewport_height(m_window->get_height());
        m_scene_renderer->begin_scene();
        m_scene_renderer->end_scene();
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::on_event(rex::events::Event& event)
    {
        m_camera_controller.on_event(event);
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::setup_scene()
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
    void DeferredRenderingLayer::setup_camera()
    {
        float viewport_width = (float)m_window->get_width();
        float viewport_height = (float)m_window->get_height();

        rex::AspectRatio aspect_ratio = rex::AspectRatio(viewport_width, viewport_height);

        float near_plane = deferred_rendering::camera_settings::NEAR_PLANE;
        float far_plane = deferred_rendering::camera_settings::FAR_PLANE;
        float fov = deferred_rendering::camera_settings::FIELD_OF_VIEW;

        m_camera.set_perspective(rex::FieldOfView(rex::DegAngle(fov), aspect_ratio.get_ratio()), rex::ClippingPlanes(near_plane, far_plane));
        m_camera.activate();

        rex::ecs::Entity camera_entity = m_scene->create_entity("camera"_sid);

        rex::ecs::TransformComponent& transform_component = camera_entity.get_component<rex::ecs::TransformComponent>();
        rex::ecs::CameraComponent& camera_component = camera_entity.add_component<rex::ecs::CameraComponent>(m_camera);

        m_camera_controller.set_camera(&camera_component.camera);
        m_camera_controller.set_camera_transform(&transform_component.transform);
    }
    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::setup_scene_renderer()
    {
        rex::SceneRenderPasses renderpasses;

        auto predepth = create_pre_depth_pass(deferred_rendering::create_pre_depth_pass_options());
        auto deferred_geometry = create_deferred_geometry_pass(deferred_rendering::create_deferred_geometry_pass_options());
        auto deferred_light = create_deferred_light_pass(deferred_rendering::create_deferred_light_pass_options());
        auto composite = create_composite_pass(deferred_rendering::create_composite_pass_options());

        renderpasses.push_back(std::move(predepth));
        renderpasses.push_back(std::move(deferred_geometry));
        renderpasses.push_back(std::move(deferred_light));
        renderpasses.push_back(std::move(composite));

        m_scene_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(renderpasses));
    }

    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_pre_depth_pass(const rex::PreDepthPassOptions& options) const
    {
        return std::make_unique<rex::PreDepthPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_deferred_geometry_pass(const rex::DeferredGeometryPassOptions& options) const
    {
        return std::make_unique<rex::DeferredGeometryPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const
    {
        return std::make_unique<rex::DeferredLightPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina