#include "regina_pch.h"

#include "forward_rendering_layer.h"

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
#include "renderpasses/forward_geometry_pass.h"
#include "renderpasses/pre_depth_pass.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/frame_buffer_pool.h"
#include "resources/uniform_buffer_set.h"
#include "resources/material.h"

#include "mesh_factory.h"
#include "model.h"

#include "model_importer.h"
#include "texture_importer.h"

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
    namespace forward_rendering
    {
        // Mesh settings
        rex::vec3 MESH_POSITION = rex::vec3(0.0f);
        rex::vec3 MESH_ROTATION = rex::radians(rex::vec3(0.0f));
        rex::vec3 MESH_SCALE = {10.0, 10.0, 10.0};

        // Render pass settings
        const rex::StringID PREDEPTHPASS_NAME = "PreDepthPass"_sid;
        const rex::StringID FORWARDGEOMETRYPASS_NAME = "ForwardGeometryPass"_sid;
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

            float MOVE_SENSITIVITY = 2.0f;
            float SCROLL_SENSITIVITY = 20.0f;
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

            options.pass_name = forward_rendering::PREDEPTHPASS_NAME;
            options.shader_name = "predepth"_sid;
            options.near_plane = forward_rendering::camera_settings::NEAR_PLANE;
            options.far_plane = forward_rendering::camera_settings::FAR_PLANE;
            options.backface_culling = forward_rendering::renderpass_settings::BACKFACE_CULLING;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::ForwardGeometryPassOptions create_forward_geometry_pass_options()
        {
            rex::ForwardGeometryPassOptions options;

            options.pass_name = forward_rendering::FORWARDGEOMETRYPASS_NAME;
            options.show_environment = forward_rendering::renderpass_settings::SHOW_ENVIRONMENT;
            options.show_grid = forward_rendering::renderpass_settings::SHOW_GRID;
            options.backface_culling = forward_rendering::renderpass_settings::BACKFACE_CULLING;
            options.geometry_shader_name = "pbr_static"_sid;
            options.skybox_shader_name = "skybox"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options()
        {
            rex::CompositePassOptions options;

            options.pass_name = forward_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = forward_rendering::FORWARDGEOMETRYPASS_NAME;
            options.gamma_correction = forward_rendering::renderpass_settings::GAMMA_CORRECTION;

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
            rex::vec3 camera_pos = forward_rendering::camera_settings::CAMERA_POSITION;
            rex::quaternion camera_rot = rex::quaternion(forward_rendering::camera_settings::CAMERA_ROTATION);

            bool can_rotate_pitch = forward_rendering::camera_settings::CAN_ROTATE_PITCH;
            bool can_rotate_yaw = forward_rendering::camera_settings::CAN_ROTATE_YAW;
            bool can_zoom = forward_rendering::camera_settings::CAN_ZOOM;

            bool initial_enabled = forward_rendering::camera_settings::IS_ENABLED;

            description.camera_settings.camera_position = camera_pos;
            description.camera_settings.camera_rotation = camera_rot;
            description.camera_settings.can_rotate_pitch = can_rotate_pitch ? CanRotatePitch::YES : CanRotatePitch::NO;
            description.camera_settings.can_rotate_yaw = can_rotate_yaw ? CanRotateYaw::YES : CanRotateYaw::NO;
            description.camera_settings.can_zoom = can_zoom ? CanZoom::YES : CanZoom::NO;
            description.camera_settings.enabled = initial_enabled;

            // Focus
            description.focus_settings = create_focus_settings(forward_rendering::camera_settings::CAMERA_FOCUS, forward_rendering::camera_settings::MIN_FOCUS_DISTANCE, forward_rendering::camera_settings::MAX_FOCUS_DISTANCE,
                                                               forward_rendering::camera_settings::FOCUS_DISTANCE);

            // Orbit
            description.orbit_settings = create_orbit_settings(forward_rendering::camera_settings::ROTATION_SPEED, forward_rendering::camera_settings::MIN_PITCH_ANGLE, forward_rendering::camera_settings::MAX_PITCH_ANGLE);

            // Mouse
            description.mouse_settings = create_mouse_settings(forward_rendering::camera_settings::MOVE_SENSITIVITY, forward_rendering::camera_settings::SCROLL_SENSITIVITY);

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
            load_shader("blit"_sid, "1000"_sid, "content\\shaders\\blit.vertex"_sid, "content\\shaders\\blit.fragment"_sid);
            load_shader("grid"_sid, "1000"_sid, "content\\shaders\\grid.vertex"_sid, "content\\shaders\\grid.fragment"_sid);
            load_shader("predepth"_sid, "1000"_sid, "content\\shaders\\predepth.vertex"_sid, "content\\shaders\\predepth.fragment"_sid);
            load_shader("skybox"_sid, "1000"_sid, "content\\shaders\\skybox.vertex"_sid, "content\\shaders\\skybox.fragment"_sid);
            load_shader("pbr_static"_sid, "1000"_sid, "content\\shaders\\pbr_static.vertex"_sid, "content\\shaders\\pbr_static.fragment"_sid);
        }
        //-------------------------------------------------------------------------
        void load_texture(const rex::StringID& name, const rex::StringID& path, const SRGB& srgb, const rex::Texture::Usage& usage)
        {
            auto texture = texture_importer::import(name, path, srgb, usage);

            if (texture == nullptr)
            {
                R_WARN("{0} texture was not found on disk", name.to_string());
                return;
            }

            rex::texture_library::add(texture);
        }
        //-------------------------------------------------------------------------
        void load_textures()
        {
            load_texture("backpack_ao", "content\\textures\\backpack_ao.jpg", SRGB::NO, rex::Texture::Usage::AMBIENT_OCCLUSION);
            load_texture("backpack_diffuse", "content\\textures\\backpack_diffuse.jpg", SRGB::NO, rex::Texture::Usage::DIFFUSE);
            load_texture("backpack_normal", "content\\textures\\backpack_normal.png", SRGB::NO, rex::Texture::Usage::NORMAL);
            load_texture("backpack_roughness", "content\\textures\\backpack_roughness.jpg", SRGB::NO, rex::Texture::Usage::ROUGHNESS);
            load_texture("backpack_specular", "content\\textures\\backpack_specular.jpg", SRGB::NO, rex::Texture::Usage::SPECUALR);
        }
        //-------------------------------------------------------------------------
        void load_primitive_geometry()
        {
            rex::mesh_factory::load();
        }
    } // namespace forward_rendering

    //-------------------------------------------------------------------------
    ForwardRenderingLayer::ForwardRenderingLayer(const rex::CoreWindow* window)
        : Layer("regina_layer"_sid, 0, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, forward_rendering::create_orbit_camera_description())
        , m_window(window)
    {
    }
    //-------------------------------------------------------------------------
    ForwardRenderingLayer::~ForwardRenderingLayer()
    {
    }

    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::on_attach()
    {
        forward_rendering::load_shaders();
        forward_rendering::load_textures();
        forward_rendering::load_primitive_geometry();
        
        m_bunny = model_importer::import("content\\meshes\\bunny.obj"_sid);
        m_unit_cube = rex::mesh_factory::get_unit_cube();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::on_detach()
    {
        m_bunny.reset();
        m_unit_cube.reset();

        rex::mesh_factory::clear();
        rex::texture_library::clear();
        rex::shader_library::clear();

        rex::FrameBufferPool::instance()->clear();
        rex::UniformBufferSet::instance()->clear();

        m_scene_renderer->destroy();
        m_scene_renderer.reset();

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::on_update(const rex::FrameInfo& info)
    {
        m_camera_controller.on_update(info);

        m_scene_renderer->set_viewport_width(m_window->get_width());
        m_scene_renderer->set_viewport_height(m_window->get_height());
        m_scene_renderer->begin_scene();
        m_scene_renderer->end_scene();
    }

    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::on_event(rex::events::Event& event)
    {
        m_camera_controller.on_event(event);
    }

    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::setup_scene()
    {
        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        setup_lights();

        // Create grey material
        //
        m_grey_material = rex::ResourceFactory::create_material(rex::shader_library::get("pbr_static"_sid), "Grey Material"_sid);
        m_grey_material->set("u_albedo"_sid, rex::vec3(0.85f, 0.85f, 0.85f));
        m_grey_material->set("u_ao"_sid, 1.0f);
        m_grey_material->set("u_roughness"_sid, 0.85f);
        m_grey_material->set("u_metallic"_sid, 0.05f);

        // Create bunny entity
        //
        rex::ecs::Entity bunny = m_scene->create_entity("bunny"_sid);

        bunny.add_component<rex::ecs::ModelComponent>(m_bunny);
        bunny.add_component<rex::ecs::MaterialComponent>(m_grey_material);

        rex::ecs::TransformComponent& bunny_transform_comp = bunny.get_component<rex::ecs::TransformComponent>();
        bunny_transform_comp.transform.set_scale(forward_rendering::MESH_SCALE);

        // Create cube entity
        rex::ecs::Entity unit_cube = m_scene->create_entity("cube"_sid);

        unit_cube.add_component<rex::ecs::ModelComponent>(m_unit_cube);
        unit_cube.add_component<rex::ecs::MaterialComponent>(m_grey_material);

        rex::ecs::TransformComponent& unit_cube_transform_comp = unit_cube.get_component<rex::ecs::TransformComponent>();
        unit_cube_transform_comp.transform.set_position(rex::vec3(-1.0f, -1.0, -1.0));
    }
    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::setup_camera()
    {
        float viewport_width = (float)m_window->get_width();
        float viewport_height = (float)m_window->get_height();

        rex::AspectRatio aspect_ratio = rex::AspectRatio(viewport_width, viewport_height);

        float near_plane = forward_rendering::camera_settings::NEAR_PLANE;
        float far_plane = forward_rendering::camera_settings::FAR_PLANE;
        float fov = forward_rendering::camera_settings::FIELD_OF_VIEW;

        m_camera.set_perspective(rex::FieldOfView(rex::DegAngle(fov), aspect_ratio.get_ratio()), rex::ClippingPlanes(near_plane, far_plane));
        m_camera.activate();

        rex::ecs::Entity camera_entity = m_scene->create_entity("camera"_sid);

        rex::ecs::TransformComponent& transform_component = camera_entity.get_component<rex::ecs::TransformComponent>();
        rex::ecs::CameraComponent& camera_component = camera_entity.add_component<rex::ecs::CameraComponent>(m_camera);

        m_camera_controller.set_camera(&camera_component.camera);
        m_camera_controller.set_camera_transform(&transform_component.transform);
    }
    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::setup_scene_renderer()
    {
        rex::SceneRenderPasses renderpasses;

        auto predepth = create_pre_depth_pass(forward_rendering::create_pre_depth_pass_options());
        auto forward_geometry = create_forward_geometry_pass(forward_rendering::create_forward_geometry_pass_options());
        auto composite = create_composite_pass(forward_rendering::create_composite_pass_options());

        renderpasses.push_back(std::move(predepth));
        renderpasses.push_back(std::move(forward_geometry));
        renderpasses.push_back(std::move(composite));

        m_scene_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(renderpasses));
    }

    //-------------------------------------------------------------------------
    void ForwardRenderingLayer::setup_lights()
    {
        const int32 NR_LIGHTS = 32;

        srand(13); // seed random number generator
        for (int32 i = 0; i < NR_LIGHTS; ++i)
        {
            std::stringstream stream;
            stream << "light_";
            stream << i;

            float x_pos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
            float y_pos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
            float z_pos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);

            float r_color = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
            float g_color = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
            float b_color = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)

            rex::ecs::Entity light = m_scene->create_entity(rex::create_sid(stream.str()));

            light.get_component<rex::ecs::TransformComponent>().transform.set_position(rex::vec3(x_pos, y_pos, z_pos));
            light.add_component<rex::ecs::PointLightComponent>(1.0f, 100.0f, 100.0f, rex::ColorRGB(r_color, g_color, b_color));
        }
    }

    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> ForwardRenderingLayer::create_pre_depth_pass(const rex::PreDepthPassOptions& options) const
    {
        return std::make_unique<rex::PreDepthPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> ForwardRenderingLayer::create_forward_geometry_pass(const rex::ForwardGeometryPassOptions& options) const
    {
        return std::make_unique<rex::ForwardGeometryPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> ForwardRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina