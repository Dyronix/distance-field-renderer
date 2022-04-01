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
#include "renderpasses/deferred_light_visualization_pass.h"
#include "renderpasses/deferred_geometry_pass.h"
#include "renderpasses/deferred_light_pass.h"
#include "renderpasses/pre_depth_pass.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/frame_buffer_pool.h"
#include "resources/uniform_buffer_set.h"
#include "resources/material.h"

#include "renderer/renderer.h"

#include "mesh_factory.h"

#include "model_library.h"
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
    // Render pass settings
    namespace deferred_rendering
    {
        // Mesh settings
        enum class MeshType
        {
            BUNNY,
            CUBE,
            CYLINDER,
            MONKEY,
            SPHERE,
            TORUS,
            DRAGON,
            TIGER,
        };

        using MeshNameMap = std::unordered_map<MeshType, rex::StringID>;
        using MeshScaleMap = std::unordered_map<MeshType, rex::vec3>;

        using MeshLattice = rex::YesNoEnum;

        MeshType MESH_TYPE = MeshType::DRAGON;
        MeshLattice MESH_LATTICE = MeshLattice::YES;

        MeshNameMap MESH_NAME_MAP =
        {
            { MeshType::BUNNY,      "Bunny"_sid },
            { MeshType::CUBE,       "Cube"_sid },
            { MeshType::CYLINDER,   "Cylinder"_sid },
            { MeshType::MONKEY,     "Monkey"_sid },
            { MeshType::SPHERE,     "Sphere"_sid },
            { MeshType::TORUS,      "Torus"_sid },
            { MeshType::DRAGON,     "Dragon"_sid },
            { MeshType::TIGER,      "Tiger"_sid },
        };
        MeshScaleMap MESH_SCALE_MAP
        {
            { MeshType::BUNNY,      {10.0, 10.0, 10.0}},
            { MeshType::CUBE,       {0.5, 0.5, 0.5}},
            { MeshType::CYLINDER,   {0.5, 0.5, 0.5}},
            { MeshType::MONKEY,     {0.5, 0.5, 0.5} },
            { MeshType::SPHERE,     {0.5, 0.5, 0.5} },
            { MeshType::TORUS,      {0.5, 0.5, 0.5} },
            { MeshType::DRAGON,     {0.02, 0.02, 0.02} },
            { MeshType::TIGER,      {1.5, 1.5, 1.5} },
        };

        // Render pass settings
        const rex::StringID PREDEPTHPASS_NAME = "PreDepthPass"_sid;
        const rex::StringID DEFERREDGEOMETRYPASS_NAME = "DeferredGeometryPass"_sid;
        const rex::StringID DEFERREDLIGHTPASS_NAME = "DeferredLightPass"_sid;
        const rex::StringID DEFERREDLIGHTVISUALIZATIONPASS_NAME = "DeferredLightVisualizationPass"_sid;
        const rex::StringID COMPOSITEPASS_NAME = "CompositePass"_sid;

        namespace camera_settings
        {
            float NEAR_PLANE = 0.01f;
            float FAR_PLANE = 1000.0f;

            float FIELD_OF_VIEW = 95.142f;

            rex::vec3 CAMERA_POSITION = {2.4f, 1.3f, 0.17f};
            rex::vec3 CAMERA_FOCUS = {0.0f, 0.0f, 0.0f};
            rex::vec3 CAMERA_ROTATION = {3.0f, 1.5f, 0.0f};

            bool CAN_ROTATE_PITCH = true;
            bool CAN_ROTATE_YAW = true;
            bool CAN_ZOOM = true;
            bool IS_ENABLED = true;

            float MIN_FOCUS_DISTANCE = 1.0f;
            float MAX_FOCUS_DISTANCE = 100.0f;
            float FOCUS_DISTANCE = 2.5f;

            float ROTATION_SPEED = 1.0f;
            float MIN_PITCH_ANGLE = -50.0f;
            float MAX_PITCH_ANGLE = 50.0f;

            float MOVE_SENSITIVITY = 2.0f;
            float SCROLL_SENSITIVITY = 5.0f;
        } // namespace camera_settings

        namespace renderpass_settings
        {
            bool BACKFACE_CULLING = true;
            bool SHOW_ENVIRONMENT = false;
            bool SHOW_GRID = true;
            float GAMMA_CORRECTION = 2.0f;
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
        rex::DeferredLightVisualizationPassOptions create_deferred_light_visualization_pass_options()
        {
            rex::DeferredLightVisualizationPassOptions options;

            options.pass_name = deferred_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME;
            options.color_pass_name = deferred_rendering::DEFERREDLIGHTPASS_NAME;
            options.depth_pass_name = deferred_rendering::DEFERREDGEOMETRYPASS_NAME;
            options.shader_name = "deferred_shading_lighting_visualization"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options()
        {
            rex::CompositePassOptions options;

            options.pass_name = deferred_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = deferred_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME;
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
            R_PROFILE_FUNCTION();

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
            R_PROFILE_FUNCTION();

            load_shader("blit"_sid, "1000"_sid, "content\\shaders\\blit.vertex"_sid, "content\\shaders\\blit.fragment"_sid);
            load_shader("g_buffer"_sid, "1000"_sid, "content\\shaders\\g_buffer.vertex"_sid, "content\\shaders\\g_buffer.fragment"_sid);
            load_shader("deferred_shading_lighting"_sid, "1000"_sid, "content\\shaders\\deferred_shading_lighting.vertex"_sid, "content\\shaders\\deferred_shading_lighting.fragment"_sid);
            load_shader("deferred_shading_lighting_visualization"_sid, "1000"_sid, "content\\shaders\\deferred_shading_lighting_visualization.vertex"_sid, "content\\shaders\\deferred_shading_lighting_visualization.fragment"_sid);
        }
        //-------------------------------------------------------------------------
        void load_texture(const rex::StringID& name, const rex::StringID& path, const SRGB& srgb, const rex::Texture::Usage& usage)
        {
            R_PROFILE_FUNCTION();

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
            R_PROFILE_FUNCTION();

            // load_texture("diffuse", "content\\textures\\diffuse.png", SRGB::NO, rex::Texture::Usage::DIFFUSE);
        }
        //-------------------------------------------------------------------------
        void load_primitive_geometry()
        {
            R_PROFILE_FUNCTION();

            rex::mesh_factory::load();
        }
        //-------------------------------------------------------------------------
        void load_custom_geometry()
        {
            R_PROFILE_FUNCTION(); 

            if (MESH_LATTICE)
            {
                switch (MESH_TYPE)
                {
                    case MeshType::BUNNY: rex::model_library::add(model_importer::import("content\\meshes\\bunny_lattice.obj"_sid, MESH_NAME_MAP[MeshType::BUNNY])); break;
                    case MeshType::CUBE: rex::model_library::add(model_importer::import("content\\meshes\\cube_lattice.obj"_sid, MESH_NAME_MAP[MeshType::CUBE])); break;
                    case MeshType::CYLINDER: rex::model_library::add(model_importer::import("content\\meshes\\cylinder_lattice.obj"_sid, MESH_NAME_MAP[MeshType::CYLINDER])); break;
                    case MeshType::MONKEY: rex::model_library::add(model_importer::import("content\\meshes\\monkey_lattice.obj"_sid, MESH_NAME_MAP[MeshType::MONKEY])); break;
                    case MeshType::SPHERE: rex::model_library::add(model_importer::import("content\\meshes\\sphere_lattice.obj"_sid, MESH_NAME_MAP[MeshType::SPHERE])); break;
                    case MeshType::TORUS: rex::model_library::add(model_importer::import("content\\meshes\\torus_lattice.obj"_sid, MESH_NAME_MAP[MeshType::TORUS])); break;
                    case MeshType::DRAGON: rex::model_library::add(model_importer::import("content\\meshes\\dragon_lattice.obj"_sid, MESH_NAME_MAP[MeshType::DRAGON])); break;
                    case MeshType::TIGER: rex::model_library::add(model_importer::import("content\\meshes\\tiger_lattice.obj"_sid, MESH_NAME_MAP[MeshType::TIGER])); break;
                }
            }
            else
            {
                switch (MESH_TYPE)
                {
                    case MeshType::BUNNY: rex::model_library::add(model_importer::import("content\\meshes\\bunny.obj"_sid, MESH_NAME_MAP[MeshType::BUNNY])); break;
                    case MeshType::CUBE: rex::model_library::add(model_importer::import("content\\meshes\\cube.obj"_sid, MESH_NAME_MAP[MeshType::CUBE])); break;
                    case MeshType::CYLINDER: rex::model_library::add(model_importer::import("content\\meshes\\cylinder.obj"_sid, MESH_NAME_MAP[MeshType::CYLINDER])); break;
                    case MeshType::MONKEY: rex::model_library::add(model_importer::import("content\\meshes\\monkey.obj"_sid, MESH_NAME_MAP[MeshType::MONKEY])); break;
                    case MeshType::SPHERE: rex::model_library::add(model_importer::import("content\\meshes\\sphere.obj"_sid, MESH_NAME_MAP[MeshType::SPHERE])); break;
                    case MeshType::TORUS: rex::model_library::add(model_importer::import("content\\meshes\\torus.obj"_sid, MESH_NAME_MAP[MeshType::TORUS])); break;
                    case MeshType::DRAGON: rex::model_library::add(model_importer::import("content\\meshes\\dragon.obj"_sid, MESH_NAME_MAP[MeshType::DRAGON])); break;
                    case MeshType::TIGER: rex::model_library::add(model_importer::import("content\\meshes\\tiger.obj"_sid, MESH_NAME_MAP[MeshType::TIGER])); break;
                }
            }
        }
    } // namespace deferred_rendering

    //-------------------------------------------------------------------------
    DeferredRenderingLayer::DeferredRenderingLayer(const rex::CoreWindow* window)
        : Layer("regina_layer"_sid, -1, EnableImGUIRendering::NO)
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
        R_PROFILE_FUNCTION();

        deferred_rendering::load_shaders();
        deferred_rendering::load_textures();
        deferred_rendering::load_primitive_geometry();
        deferred_rendering::load_custom_geometry();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::on_detach()
    {
        R_PROFILE_FUNCTION();

        rex::model_library::clear();
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
    void DeferredRenderingLayer::on_update(const rex::FrameInfo& info)
    {
        R_PROFILE_FUNCTION();

        m_camera_controller.on_update(info);

        m_scene->update();

        m_scene_renderer->set_viewport_width(m_window->get_width());
        m_scene_renderer->set_viewport_height(m_window->get_height());
        m_scene_renderer->begin_scene();
        m_scene_renderer->end_scene();
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::on_event(rex::events::Event& event)
    {
        R_PROFILE_FUNCTION();

        m_camera_controller.on_event(event);
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::setup_scene()
    {
        R_PROFILE_FUNCTION();

        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        // Create grey material
        //
        m_bunny_material = rex::ResourceFactory::create_material(rex::shader_library::get("g_buffer"), "G Buffer Material"_sid);
        m_bunny_material->set_texture2d("u_Texture_Diffuse", rex::Renderer::get_white_texture());
        m_bunny_material->set_texture2d("u_Texture_Specular", rex::Renderer::get_black_texture());

        setup_lights();
        setup_geometry();
    }
    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::setup_camera()
    {
        R_PROFILE_FUNCTION();

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
        R_PROFILE_FUNCTION();

        rex::SceneRenderPasses renderpasses;

        auto deferred_geometry = create_deferred_geometry_pass(deferred_rendering::create_deferred_geometry_pass_options());
        auto deferred_light = create_deferred_light_pass(deferred_rendering::create_deferred_light_pass_options());
        auto deferred_light_visualization = create_deferred_light_visualization_pass(deferred_rendering::create_deferred_light_visualization_pass_options());
        auto composite = create_composite_pass(deferred_rendering::create_composite_pass_options());

        renderpasses.push_back(std::move(deferred_geometry));
        renderpasses.push_back(std::move(deferred_light));
        renderpasses.push_back(std::move(deferred_light_visualization));
        renderpasses.push_back(std::move(composite));

        m_scene_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(renderpasses));
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::setup_lights()
    {
        R_PROFILE_FUNCTION();

        srand(13);  // seed random number generator
        for (int32 i = 0; i < 32; ++i)
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
            light.add_component<rex::ecs::PointLightComponent>(1.0f, 1.0f, 0.7f, 1.8f, rex::ColorRGB(r_color, g_color, b_color));
        }
    }

    //-------------------------------------------------------------------------
    void DeferredRenderingLayer::setup_geometry()
    {
        R_PROFILE_FUNCTION();

        const int32 MAX_OBJECTS = 9;

        std::array<rex::vec3, MAX_OBJECTS> object_positions = 
        {
            rex::vec3(-3.0, -0.0, -3.0),
            rex::vec3(0.0, -0.0, -3.0),
            rex::vec3(3.0, -0.0, -3.0),
            rex::vec3(-3.0, -0.0, 0.0),
            rex::vec3(0.0, -0.0, 0.0),
            rex::vec3(3.0, -0.0, 0.0),
            rex::vec3(-3.0, -0.0, 3.0),
            rex::vec3(0.0, -0.0, 3.0),
            rex::vec3(3.0, -0.0, 3.0)
        };

        auto mesh_type = deferred_rendering::MESH_TYPE;
        auto mesh_name = deferred_rendering::MESH_NAME_MAP[mesh_type];
        auto mesh_scale = deferred_rendering::MESH_SCALE_MAP[mesh_type];

        rex::ref_ptr<rex::Model> model = rex::model_library::get(mesh_name);
        if (model == nullptr)
        {
            return;
        }

        for (int32 i = 0; i < MAX_OBJECTS; ++i)
        {
            rex::ecs::Entity entity = m_scene->create_entity(mesh_name);

            entity.add_component<rex::ecs::ModelComponent>(model);
            entity.add_component<rex::ecs::MaterialComponent>(m_bunny_material);

            rex::ecs::TransformComponent& transform_comp = entity.get_component<rex::ecs::TransformComponent>();

            transform_comp.transform.set_position(object_positions[i]);
            transform_comp.transform.set_scale(mesh_scale);

            m_bunny_entities.push_back(entity);
        }
    }

    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_pre_depth_pass(const rex::PreDepthPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::PreDepthPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_deferred_geometry_pass(const rex::DeferredGeometryPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::DeferredGeometryPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::DeferredLightPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::DeferredLightVisualizationPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DeferredRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina