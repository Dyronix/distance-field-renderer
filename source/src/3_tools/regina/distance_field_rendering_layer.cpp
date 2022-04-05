#include "regina_pch.h"

#include "distance_field_rendering_layer.h" 

#include "ecs/components/camera_component.h"
#include "ecs/components/directional_light_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/tag_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/entity.h"
#include "ecs/scene.h"

#include "renderpasses/blur_pass.h"
#include "renderpasses/clear_pass.h"
#include "renderpasses/composite_pass.h"
#include "renderpasses/distance_evaluation_pass.h"
#include "renderpasses/deferred_light_pass.h"
#include "renderpasses/deferred_light_visualization_pass.h"
#include "renderpasses/pre_depth_pass.h"

#include "resources/frame_buffer_pool.h"
#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/uniform_buffer_set.h"

#include "renderer/renderer.h"

#include "volume_importer.h"
#include "volume_library.h"

#include "mesh_factory.h"

#include "scene_renderer.h"

#include "core_window.h"
#include "core_application.h"

#include "event_dispatcher.h"

#include "input/key_pressed.h"
#include "input/keyboard/win32_key_codes.h"
#include "input/win32_input.h"

#include "aspect_ratio.h"
#include "field_of_view.h"

#include "input/mouse/win32_mouse_codes.h"
#include "input/win32_input.h"

#include "resources/shader_program.h"

#include "file_import.h"
#include "file_memory.h"

namespace regina
{
    // Render pass settings
    namespace distance_field_rendering
    {
        // Mesh settings
        enum class VolumeType
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

        using VolumeNameMap = std::unordered_map<VolumeType, rex::StringID>;
        using VolumeLattice = rex::YesNoEnum;

        VolumeType VOLUME_TYPE = VolumeType::TIGER;
        VolumeLattice VOLUME_LATTICE = VolumeLattice::YES;

        //-------------------------------------------------------------------------
        VolumeNameMap& get_volume_name_map()
        {
            static VolumeNameMap VOLUME_NAME_MAP =
            {
                { VolumeType::BUNNY,      "Bunny"_sid },
                { VolumeType::CUBE,       "Cube"_sid },
                { VolumeType::CYLINDER,   "Cylinder"_sid },
                { VolumeType::MONKEY,     "Monkey"_sid },
                { VolumeType::SPHERE,     "Sphere"_sid },
                { VolumeType::TORUS,      "Torus"_sid },
                { VolumeType::DRAGON,     "Dragon"_sid },
                { VolumeType::TIGER,      "Tiger"_sid },
            };

            return VOLUME_NAME_MAP;
        }
        //-------------------------------------------------------------------------
        std::unordered_map<VolumeType, float>& get_volume_scale_map()
        {
            static std::unordered_map<VolumeType, float> MAP
            {
                { VolumeType::BUNNY,      1.0f},
                { VolumeType::CUBE,       1.0f},
                { VolumeType::CYLINDER,   1.0f},
                { VolumeType::MONKEY,     1.0f},
                { VolumeType::SPHERE,     1.0f},
                { VolumeType::TORUS,      1.0f},
                { VolumeType::DRAGON,     1.0f},
                { VolumeType::TIGER,      1.0f},
            };

            return MAP;
        }
        //-------------------------------------------------------------------------
        std::unordered_map<VolumeType, float>& get_volume_offset_map()
        {
            static std::unordered_map<VolumeType, float> MAP
            {
                { VolumeType::BUNNY,      -0.001f},
                { VolumeType::CUBE,       -0.001f},
                { VolumeType::CYLINDER,   -0.001f},
                { VolumeType::MONKEY,     -0.001f},
                { VolumeType::SPHERE,     -0.001f},
                { VolumeType::TORUS,      -0.001f},
                { VolumeType::DRAGON,     -0.001f},
                { VolumeType::TIGER,      -0.001f},
            };

            return MAP;
        }

        // Render pass settings
        const rex::StringID PREDEPTHPASS_NAME = "PreDepthPass"_sid;
        const rex::StringID DISTANCEEVALUATIONSPASS_NAME = "DistanceEvaluationsPass"_sid;
        const rex::StringID DEFERREDLIGHTPASS_NAME = "DeferredLightPass"_sid;
        const rex::StringID DEFERREDLIGHTVISUALIZATIONPASS_NAME = "DeferredLightVisualizationPass"_sid;
        const rex::StringID COMPOSITEPASS_NAME = "CompositePass"_sid;

        namespace camera_settings
        {
            float NEAR_PLANE = 0.01f;
            float FAR_PLANE = 10.0f;

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
            
            int32 MAX_SPHERE_TRACER_ITERATIONS = 150;
            float MAX_MARCH_DISTANCE = 100.0f;
            float MIN_SURFACE_DISTANCE = 0.01f;
        } // namespace renderpass_settings

        //-------------------------------------------------------------------------
        rex::vec3 calculate_scene_size(const Volume& volume)
        {
            rex::vec3 voxel_grid_size = volume.get_voxel_grid_bounds().maximum - volume.get_voxel_grid_bounds().minimum;

            float longest_edge = rex::max_coeff(voxel_grid_size);

            float x_width = rex::remap(voxel_grid_size.x, 0.0f, 0.0f, longest_edge, 1.0f);
            float y_width = rex::remap(voxel_grid_size.y, 0.0f, 0.0f, longest_edge, 1.0f);
            float z_width = rex::remap(voxel_grid_size.z, 0.0f, 0.0f, longest_edge, 1.0f);

            return rex::vec3(x_width, y_width, z_width);
        }

        //-------------------------------------------------------------------------
        rex::PreDepthPassOptions create_pre_depth_pass_options()
        {
            rex::PreDepthPassOptions options;

            options.pass_name = distance_field_rendering::PREDEPTHPASS_NAME;
            options.shader_name = "predepth"_sid;
            options.near_plane = distance_field_rendering::camera_settings::NEAR_PLANE;
            options.far_plane = distance_field_rendering::camera_settings::FAR_PLANE;
            options.backface_culling = distance_field_rendering::renderpass_settings::BACKFACE_CULLING;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DistanceEvaluationsPassOptions create_distance_evaluation_pass_options()
        {
            rex::DistanceEvaluationsPassOptions options;

            options.pass_name = distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME;
            options.shader_name = "g_buffer_distance_field"_sid;

            options.sphere_tracer_options.max_iterations = renderpass_settings::MAX_SPHERE_TRACER_ITERATIONS;
            options.sphere_tracer_options.max_march_distance = renderpass_settings::MAX_MARCH_DISTANCE;
            options.sphere_tracer_options.min_surface_distance = renderpass_settings::MIN_SURFACE_DISTANCE;

            const Volume& volume = volume_library::get_volume(get_volume_name_map()[VOLUME_TYPE]);
            const VolumeMeta& volume_meta = volume.get_volume_meta();

            const float scene_scale = get_volume_scale_map()[VOLUME_TYPE];
            const float scene_offset = get_volume_offset_map()[VOLUME_TYPE];

            options.sdf_scene_options.scene_scale = scene_scale;
            options.sdf_scene_options.scene_offset = scene_offset - renderpass_settings::MIN_SURFACE_DISTANCE;
            options.sdf_scene_options.scene_size = calculate_scene_size(volume) * 0.5f;
            options.sdf_scene_options.scene_center = rex::vec3(0.0f, 0.0f, 0.0f);

            options.sdf_scene_options.scene_voxel_grid_min_bounds = volume_meta.voxel_grid_bounds.minimum;
            options.sdf_scene_options.scene_voxel_grid_max_bounds = volume_meta.voxel_grid_bounds.maximum;
            options.sdf_scene_options.scene_voxel_grid_size = volume_meta.voxel_grid_size;
            options.sdf_scene_options.scene_voxel_grid_cell_size = volume_meta.voxel_grid_cell_size;

            options.sdf_scene_options.scene_data = volume_library::get_volume_data(get_volume_name_map()[VOLUME_TYPE]);

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightPassOptions create_deferred_light_pass_options()
        {
            rex::DeferredLightPassOptions options;

            options.pass_name = distance_field_rendering::DEFERREDLIGHTPASS_NAME;
            options.shader_name = "deferred_shading_lighting"_sid;
            options.g_position_buffer = {distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME, 0};
            options.g_normal_buffer = {distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME, 1};
            options.g_albedo_spec_buffer = {distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME, 2};

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightVisualizationPassOptions create_deferred_light_visualization_pass_options()
        {
            rex::DeferredLightVisualizationPassOptions options;

            options.pass_name = distance_field_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME;
            options.color_pass_name = distance_field_rendering::DEFERREDLIGHTPASS_NAME;
            options.depth_pass_name = distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME;
            options.shader_name = "deferred_shading_lighting_visualization"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options()
        {
            rex::CompositePassOptions options;

            options.pass_name = distance_field_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = distance_field_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME;
            options.gamma_correction = distance_field_rendering::renderpass_settings::GAMMA_CORRECTION;

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
            rex::vec3 camera_pos = distance_field_rendering::camera_settings::CAMERA_POSITION;
            rex::quaternion camera_rot = rex::quaternion(distance_field_rendering::camera_settings::CAMERA_ROTATION);

            bool can_rotate_pitch = distance_field_rendering::camera_settings::CAN_ROTATE_PITCH;
            bool can_rotate_yaw = distance_field_rendering::camera_settings::CAN_ROTATE_YAW;
            bool can_zoom = distance_field_rendering::camera_settings::CAN_ZOOM;

            bool initial_enabled = distance_field_rendering::camera_settings::IS_ENABLED;

            description.camera_settings.camera_position = camera_pos;
            description.camera_settings.camera_rotation = camera_rot;
            description.camera_settings.can_rotate_pitch = can_rotate_pitch ? CanRotatePitch::YES : CanRotatePitch::NO;
            description.camera_settings.can_rotate_yaw = can_rotate_yaw ? CanRotateYaw::YES : CanRotateYaw::NO;
            description.camera_settings.can_zoom = can_zoom ? CanZoom::YES : CanZoom::NO;
            description.camera_settings.enabled = initial_enabled;

            // Focus
            description.focus_settings = create_focus_settings(distance_field_rendering::camera_settings::CAMERA_FOCUS, distance_field_rendering::camera_settings::MIN_FOCUS_DISTANCE, distance_field_rendering::camera_settings::MAX_FOCUS_DISTANCE,
                                                               distance_field_rendering::camera_settings::FOCUS_DISTANCE);

            // Orbit
            description.orbit_settings = create_orbit_settings(distance_field_rendering::camera_settings::ROTATION_SPEED, distance_field_rendering::camera_settings::MIN_PITCH_ANGLE, distance_field_rendering::camera_settings::MAX_PITCH_ANGLE);

            // Mouse
            description.mouse_settings = create_mouse_settings(distance_field_rendering::camera_settings::MOVE_SENSITIVITY, distance_field_rendering::camera_settings::SCROLL_SENSITIVITY);

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
            load_shader("g_buffer_distance_field"_sid, "1000"_sid, "content\\shaders\\g_buffer_distance_field.vertex"_sid, "content\\shaders\\g_buffer_distance_field.fragment"_sid);
            load_shader("deferred_shading_lighting"_sid, "1000"_sid, "content\\shaders\\deferred_shading_lighting.vertex"_sid, "content\\shaders\\deferred_shading_lighting.fragment"_sid);
            load_shader("deferred_shading_lighting_visualization"_sid, "1000"_sid, "content\\shaders\\deferred_shading_lighting_visualization.vertex"_sid, "content\\shaders\\deferred_shading_lighting_visualization.fragment"_sid);
        }
        //-------------------------------------------------------------------------
        void load_primitive_geometry()
        {
            R_PROFILE_FUNCTION();

            rex::mesh_factory::load();
        }
        //-------------------------------------------------------------------------
        void load_volume(const rex::StringID& name, const rex::StringID& volumeMetaPath, const rex::StringID& volumeDataPath)
        {
            R_PROFILE_FUNCTION();

            Volume volume = volume_importer::import(name, volumeMetaPath, volumeDataPath);

            if(volume.get_volume_data().get_size() == 0)
            {
                R_ERROR("[VOLUME] Volume with name: {0}, was not imported correctly", volume.get_name().to_string());
                R_ERROR("[VOLUME] Import failed: {0}", volume.get_name().to_string());
                return;
            }

            volume_library::add(std::move(volume));

            R_INFO("[VOLUME] Import completed: {0}", name.to_string());
        }
        //-------------------------------------------------------------------------
        void load_volumes()
        {
            R_PROFILE_FUNCTION();

            load_volume(get_volume_name_map()[VolumeType::BUNNY], "content\\volumes\\bunny.sdf.meta"_sid, "content\\volumes\\bunny.sdf"_sid);
            //load_volume(get_volume_name_map()[VolumeType::CUBE], "content\\volumes\\cube.sdf.meta"_sid, "content\\volumes\\cube.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::CYLINDER], "content\\volumes\\cylinder.sdf.meta"_sid, "content\\volumes\\cylinder.sdf"_sid);
            //load_volume(get_volume_name_map()[VolumeType::DRAGON], "content\\volumes\\dragon.sdf.meta"_sid, "content\\volumes\\dragon.sdf"_sid);
            //load_volume(get_volume_name_map()[VolumeType::MONKEY], "content\\volumes\\monkey.sdf.meta"_sid, "content\\volumes\\monkey.sdf"_sid);
            //load_volume(get_volume_name_map()[VolumeType::SPHERE], "content\\volumes\\sphere.sdf.meta"_sid, "content\\volumes\\sphere.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::TIGER], "content\\volumes\\tiger.sdf.meta"_sid, "content\\volumes\\tiger.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::TORUS], "content\\volumes\\torus.sdf.meta"_sid, "content\\volumes\\torus.sdf"_sid);
        }
    } // namespace distance_field_rendering

    //-------------------------------------------------------------------------
    DistanceFieldRenderingLayer::DistanceFieldRenderingLayer(const rex::CoreWindow* window)
        : Layer("regina_layer"_sid, -1, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, distance_field_rendering::create_orbit_camera_description())
        , m_window(window)
    {
    }
    //-------------------------------------------------------------------------
    DistanceFieldRenderingLayer::~DistanceFieldRenderingLayer()
    {
    }

    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::on_attach()
    {
        R_PROFILE_FUNCTION();

        distance_field_rendering::load_volumes();
        distance_field_rendering::load_shaders();
        distance_field_rendering::load_primitive_geometry();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::on_detach()
    {
        R_PROFILE_FUNCTION();

        rex::mesh_factory::clear(); 
        rex::shader_library::clear();

        volume_library::clear();

        rex::FrameBufferPool::instance()->clear();
        rex::UniformBufferSet::instance()->clear();

        m_scene_renderer->destroy();
        m_scene_renderer.reset();

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::on_update(const rex::FrameInfo& info)
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
    void DistanceFieldRenderingLayer::on_event(rex::events::Event& event)
    {
        R_PROFILE_FUNCTION();

        m_camera_controller.on_event(event);

        rex::events::EventDispatcher dispatcher(event);

        dispatcher.dispatch<rex::events::KeyPressed>([&](rex::events::KeyPressed& pressedEvt) { return on_key_pressed(pressedEvt); });
    }

    //-------------------------------------------------------------------------
    bool DistanceFieldRenderingLayer::on_key_pressed(const rex::events::KeyPressed& keyPressEvent)
    {
        switch (keyPressEvent.get_key())
        {
            case R_KEY_UP: increment_sdf_scale(); return true;
            case R_KEY_LEFT: increment_sdf_offset(); return true;
            case R_KEY_DOWN: decrement_sdf_scale(); return true;
            case R_KEY_RIGHT: decrement_sdf_offset(); return true;
            default: return false;
        }
    }

    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::decrement_sdf_scale()
    {
        rex::SceneRenderPass* render_pass = m_scene_renderer->get_scene_render_pass(distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME);
        if (render_pass == nullptr)
        {
            return;
        }

        rex::DistanceEvaluationPass* distance_eval = static_cast<rex::DistanceEvaluationPass*>(render_pass);
        rex::sdf::SceneOptions sdf_scene_options = distance_eval->get_sdf_scene_options();

        float scene_scale = 0.0f;
        rex::vec3 scene_size = rex::zero_vec3<float>();

        {
            scene_scale = sdf_scene_options.scene_scale;
            scene_scale = std::clamp(scene_scale - 0.5f, 1.0f, 10.0f);
        }

        {
            const Volume& volume = volume_library::get_volume(distance_field_rendering::get_volume_name_map()[distance_field_rendering::VOLUME_TYPE]);

            scene_size = distance_field_rendering::calculate_scene_size(volume);
            scene_size = scene_size * scene_scale;
        }
        
        sdf_scene_options.scene_scale = scene_scale;
        sdf_scene_options.scene_size = scene_size;

        R_INFO("Scale SDF, scaling down SDF: {0}", sdf_scene_options.scene_scale);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }
    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::decrement_sdf_offset()
    {
        rex::SceneRenderPass* render_pass = m_scene_renderer->get_scene_render_pass(distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME);
        if (render_pass == nullptr)
        {
            return;
        }

        rex::DistanceEvaluationPass* distance_eval = static_cast<rex::DistanceEvaluationPass*>(render_pass);
        rex::sdf::SceneOptions sdf_scene_options = distance_eval->get_sdf_scene_options();

        float offset = 0.0f;

        offset = sdf_scene_options.scene_offset;
        offset = std::clamp(offset - 0.001f, -1.0f, 1.0f);

        sdf_scene_options.scene_offset = offset;

        R_INFO("Scale SDF, Decreasing offset SDF: {0}", sdf_scene_options.scene_offset);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }

    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::increment_sdf_scale()
    {
        rex::SceneRenderPass* render_pass = m_scene_renderer->get_scene_render_pass(distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME);
        if (render_pass == nullptr)
        {
            return;
        }

        rex::DistanceEvaluationPass* distance_eval = static_cast<rex::DistanceEvaluationPass*>(render_pass);
        rex::sdf::SceneOptions sdf_scene_options = distance_eval->get_sdf_scene_options();

        float scene_scale = 0.0f;
        rex::vec3 scene_size = rex::zero_vec3<float>();

        {
            scene_scale = sdf_scene_options.scene_scale;
            scene_scale = std::clamp(scene_scale + 0.5f, 1.0f, 10.0f);
        }

        {
            const Volume& volume = volume_library::get_volume(distance_field_rendering::get_volume_name_map()[distance_field_rendering::VOLUME_TYPE]);

            scene_size = distance_field_rendering::calculate_scene_size(volume);
            scene_size = scene_size * scene_scale;
        }

        sdf_scene_options.scene_scale = scene_scale;
        sdf_scene_options.scene_size = scene_size;

        R_INFO("Scale SDF, scaling up SDF: {0}", sdf_scene_options.scene_scale);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }
    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::increment_sdf_offset()
    {
        rex::SceneRenderPass* render_pass = m_scene_renderer->get_scene_render_pass(distance_field_rendering::DISTANCEEVALUATIONSPASS_NAME);
        if (render_pass == nullptr)
        {
            return;
        }

        rex::DistanceEvaluationPass* distance_eval = static_cast<rex::DistanceEvaluationPass*>(render_pass);
        rex::sdf::SceneOptions sdf_scene_options = distance_eval->get_sdf_scene_options();

        float offset = 0.0f;

        offset = sdf_scene_options.scene_offset;
        offset = std::clamp(offset + 0.001f, -1.0f, 1.0f);

        sdf_scene_options.scene_offset = offset;

        R_INFO("Offset SDF, Increasing offset SDF: {0}", sdf_scene_options.scene_offset);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }

    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::setup_scene()
    {
        R_PROFILE_FUNCTION();

        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        setup_lights();
    }
    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::setup_camera()
    {
        R_PROFILE_FUNCTION();

        float viewport_width = (float)m_window->get_width();
        float viewport_height = (float)m_window->get_height();

        rex::AspectRatio aspect_ratio = rex::AspectRatio(viewport_width, viewport_height);

        float near_plane = distance_field_rendering::camera_settings::NEAR_PLANE;
        float far_plane = distance_field_rendering::camera_settings::FAR_PLANE;
        float fov = distance_field_rendering::camera_settings::FIELD_OF_VIEW;

        m_camera.set_perspective(rex::FieldOfView(rex::DegAngle(fov), aspect_ratio.get_ratio()), rex::ClippingPlanes(near_plane, far_plane));
        m_camera.activate();

        rex::ecs::Entity camera_entity = m_scene->create_entity("camera"_sid);

        rex::ecs::TransformComponent& transform_component = camera_entity.get_component<rex::ecs::TransformComponent>();
        rex::ecs::CameraComponent& camera_component = camera_entity.add_component<rex::ecs::CameraComponent>(m_camera);

        m_camera_controller.set_camera(&camera_component.camera);
        m_camera_controller.set_camera_transform(&transform_component.transform);
    }
    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::setup_scene_renderer()
    {
        R_PROFILE_FUNCTION();

        rex::SceneRenderPasses renderpasses;

        auto distance_eval = create_distance_evaluation_pass(distance_field_rendering::create_distance_evaluation_pass_options());
        auto deferred_light = create_deferred_light_pass(distance_field_rendering::create_deferred_light_pass_options());
        auto deferred_light_visualization = create_deferred_light_visualization_pass(distance_field_rendering::create_deferred_light_visualization_pass_options());
        auto composite = create_composite_pass(distance_field_rendering::create_composite_pass_options());

        renderpasses.push_back(std::move(distance_eval));
        renderpasses.push_back(std::move(deferred_light));
        renderpasses.push_back(std::move(deferred_light_visualization));
        renderpasses.push_back(std::move(composite));

        m_scene_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(renderpasses));
    }

    //-------------------------------------------------------------------------
    void DistanceFieldRenderingLayer::setup_lights()
    {
        R_PROFILE_FUNCTION();

        srand(13); // seed random number generator
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
    std::unique_ptr<rex::SceneRenderPass> DistanceFieldRenderingLayer::create_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::DistanceEvaluationPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DistanceFieldRenderingLayer::create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::DeferredLightPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DistanceFieldRenderingLayer::create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::DeferredLightVisualizationPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> DistanceFieldRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina