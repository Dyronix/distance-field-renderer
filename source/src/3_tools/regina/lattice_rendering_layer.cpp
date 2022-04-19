#include "regina_pch.h"

#include "lattice_rendering_layer.h"

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
#include "renderpasses/deferred_light_pass.h"
#include "renderpasses/deferred_light_visualization_pass.h"
#include "renderpasses/lattice_distance_evaluation_pass.h"
#include "renderpasses/pre_depth_pass.h"

#include "resources/frame_buffer_pool.h"
#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/uniform_buffer_set.h"

#include "renderer/renderer.h"

#include "lattice_importer.h"
#include "lattice_library.h"

#include "volume_importer.h"
#include "volume_library.h"

#include "texture_importer.h"

#include "mesh_factory.h"

#include "scene_renderer.h"

#include "core_application.h"
#include "core_window.h"

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
    namespace lattice_rendering
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
        const rex::StringID DISTANCEEVALUATIONSPASS_NAME = "DistanceEvaluationsPass"_sid;
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
        rex::vec3 calculate_scene_size(const Lattice& lattice)
        {
            rex::vec3 lattice_grid_size = lattice.get_voxel_grid_bounds().maximum - lattice.get_voxel_grid_bounds().minimum;

            float longest_edge = rex::max_coeff(lattice_grid_size);

            float x_width = rex::remap(lattice_grid_size.x, 0.0f, 0.0f, longest_edge, 1.0f);
            float y_width = rex::remap(lattice_grid_size.y, 0.0f, 0.0f, longest_edge, 1.0f);
            float z_width = rex::remap(lattice_grid_size.z, 0.0f, 0.0f, longest_edge, 1.0f);

            return rex::vec3(x_width, y_width, z_width);
        }

        //-------------------------------------------------------------------------
        rex::DistanceEvaluationsPassOptions create_distance_evaluation_pass_options()
        {
            rex::DistanceEvaluationsPassOptions options;

            options.pass_name = lattice_rendering::DISTANCEEVALUATIONSPASS_NAME;
            options.shader_name = "heatmap"_sid;

            options.sphere_tracer_options.max_iterations = renderpass_settings::MAX_SPHERE_TRACER_ITERATIONS;
            options.sphere_tracer_options.max_march_distance = renderpass_settings::MAX_MARCH_DISTANCE;
            options.sphere_tracer_options.min_surface_distance = renderpass_settings::MIN_SURFACE_DISTANCE;

            const Lattice& lattice = lattice_library::get_lattice(get_volume_name_map()[VOLUME_TYPE]);
            const LatticeMeta& lattice_meta = lattice.get_lattice_meta();

            const float scene_scale = get_volume_scale_map()[VOLUME_TYPE];
            const float scene_offset = get_volume_offset_map()[VOLUME_TYPE];

            options.sdf_scene_options.scene_scale = scene_scale;
            options.sdf_scene_options.scene_offset = scene_offset - renderpass_settings::MIN_SURFACE_DISTANCE;
            options.sdf_scene_options.scene_size = calculate_scene_size(lattice) * 0.5f;
            options.sdf_scene_options.scene_center = rex::vec3(0.0f, 0.0f, 0.0f);

            options.sdf_scene_options.scene_voxel_grid_min_bounds = lattice_meta.voxel_grid_bounds.minimum;
            options.sdf_scene_options.scene_voxel_grid_max_bounds = lattice_meta.voxel_grid_bounds.maximum;
            options.sdf_scene_options.scene_voxel_grid_size = lattice_meta.voxel_grid_size;
            options.sdf_scene_options.scene_voxel_grid_cell_size = lattice_meta.voxel_grid_cell_size;

            options.sdf_scene_options.scene_data = lattice_library::get_lattice_data(get_volume_name_map()[VOLUME_TYPE]);

            return options;
        }

        //-------------------------------------------------------------------------
        rex::LatticeOptions create_lattice_options()
        {
            const Lattice& lattice = lattice_library::get_lattice(get_volume_name_map()[VOLUME_TYPE]);
            const LatticeMeta& lattice_meta = lattice.get_lattice_meta();

            rex::LatticeOptions options;

            options.lattice_strud_thickness = lattice_meta.lattice_strud_thickness;
            options.lattice_grid_size = lattice_meta.lattice_grid_size;
            options.lattice_grid_cell_size = lattice_meta.lattice_grid_cell_size;
            options.lattice_grid_min_bounds = lattice_meta.voxel_grid_bounds.minimum;
            options.lattice_grid_max_bounds = lattice_meta.voxel_grid_bounds.maximum;
            options.lattice_data_name = get_volume_name_map()[VOLUME_TYPE];

            return options;
        }

        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options()
        {
            rex::CompositePassOptions options;

            options.pass_name = lattice_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = lattice_rendering::DISTANCEEVALUATIONSPASS_NAME;
            options.apply_gamma_correction = rex::ApplyGammaCorrection::NO;
            options.apply_tone_mapping = rex::ApplyToneMapping::NO;

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
            rex::vec3 camera_pos = lattice_rendering::camera_settings::CAMERA_POSITION;
            rex::quaternion camera_rot = rex::quaternion(lattice_rendering::camera_settings::CAMERA_ROTATION);

            bool can_rotate_pitch = lattice_rendering::camera_settings::CAN_ROTATE_PITCH;
            bool can_rotate_yaw = lattice_rendering::camera_settings::CAN_ROTATE_YAW;
            bool can_zoom = lattice_rendering::camera_settings::CAN_ZOOM;

            bool initial_enabled = lattice_rendering::camera_settings::IS_ENABLED;

            description.camera_settings.camera_position = camera_pos;
            description.camera_settings.camera_rotation = camera_rot;
            description.camera_settings.can_rotate_pitch = can_rotate_pitch ? CanRotatePitch::YES : CanRotatePitch::NO;
            description.camera_settings.can_rotate_yaw = can_rotate_yaw ? CanRotateYaw::YES : CanRotateYaw::NO;
            description.camera_settings.can_zoom = can_zoom ? CanZoom::YES : CanZoom::NO;
            description.camera_settings.enabled = initial_enabled;

            // Focus
            description.focus_settings = create_focus_settings(lattice_rendering::camera_settings::CAMERA_FOCUS, lattice_rendering::camera_settings::MIN_FOCUS_DISTANCE, lattice_rendering::camera_settings::MAX_FOCUS_DISTANCE,
                                                               lattice_rendering::camera_settings::FOCUS_DISTANCE);

            // Orbit
            description.orbit_settings = create_orbit_settings(lattice_rendering::camera_settings::ROTATION_SPEED, lattice_rendering::camera_settings::MIN_PITCH_ANGLE, lattice_rendering::camera_settings::MAX_PITCH_ANGLE);

            // Mouse
            description.mouse_settings = create_mouse_settings(lattice_rendering::camera_settings::MOVE_SENSITIVITY, lattice_rendering::camera_settings::SCROLL_SENSITIVITY);

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
            load_shader("lattice"_sid, "1000"_sid, "content\\shaders\\lattice.vertex"_sid, "content\\shaders\\lattice.fragment"_sid);
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

            R_INFO("[TEXTURE] Import completed: {0}", name.to_string());
        }
        //-------------------------------------------------------------------------
        void load_lattice(const rex::StringID& name, const rex::StringID& latticeMetaPath, const rex::StringID& latticeDataPath)
        {
            R_PROFILE_FUNCTION();

            Lattice lattice = lattice_importer::import(name, latticeMetaPath, latticeDataPath);

            if (lattice.get_lattice_data().get_size() == 0)
            {
                R_ERROR("[LATTICE] Lattice with name: {0}, was not imported correctly", lattice.get_name());
                return;
            }

            lattice_library::add(std::move(lattice));

            R_INFO("[LATTICE] Import completed: {0}", name.to_string());
        }
        //-------------------------------------------------------------------------
        void load_lattices()
        {
            R_PROFILE_FUNCTION();
            
            load_lattice(get_volume_name_map()[VolumeType::BUNNY], "content\\lattices\\bunny.lattice.meta"_sid, "content\\lattices\\bunny.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::CYLINDER], "content\\lattices\\cylinder.lattice.meta"_sid, "content\\lattices\\cylinder.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::TIGER], "content\\lattices\\tiger.lattice.meta"_sid, "content\\lattices\\tiger.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::TORUS], "content\\lattices\\torus.lattice.meta"_sid, "content\\lattices\\torus.lattice"_sid);
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

            if (volume.get_volume_data().get_size() == 0)
            {
                R_ERROR("[VOLUME] Volume with name: {0}, was not imported correctly", volume.get_name());
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
            load_volume(get_volume_name_map()[VolumeType::CYLINDER], "content\\volumes\\cylinder.sdf.meta"_sid, "content\\volumes\\cylinder.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::TIGER], "content\\volumes\\tiger.sdf.meta"_sid, "content\\volumes\\tiger.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::TORUS], "content\\volumes\\torus.sdf.meta"_sid, "content\\volumes\\torus.sdf"_sid);
        }
    } // namespace lattice_rendering

    //-------------------------------------------------------------------------
    LatticeRenderingLayer::LatticeRenderingLayer(const rex::CoreWindow* window)
        : Layer("regina_layer"_sid, -1, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, lattice_rendering::create_orbit_camera_description())
        , m_window(window)
    {
    }
    //-------------------------------------------------------------------------
    LatticeRenderingLayer::~LatticeRenderingLayer()
    {
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_attach()
    {
        R_PROFILE_FUNCTION();

        lattice_rendering::load_volumes();
        lattice_rendering::load_lattices();
        lattice_rendering::load_shaders();
        lattice_rendering::load_primitive_geometry();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_detach()
    {
        R_PROFILE_FUNCTION();

        rex::mesh_factory::clear();
        rex::shader_library::clear();

        lattice_library::clear();
        volume_library::clear();

        rex::FrameBufferPool::instance()->clear();
        rex::UniformBufferSet::instance()->clear();

        m_scene_renderer->destroy();
        m_scene_renderer.reset();

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_update(const rex::FrameInfo& info)
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
    void LatticeRenderingLayer::on_event(rex::events::Event& event)
    {
        R_PROFILE_FUNCTION();

        m_camera_controller.on_event(event);

        rex::events::EventDispatcher dispatcher(event);

        dispatcher.dispatch<rex::events::KeyPressed>([&](rex::events::KeyPressed& pressedEvt) { return on_key_pressed(pressedEvt); });
    }

    //-------------------------------------------------------------------------
    bool LatticeRenderingLayer::on_key_pressed(const rex::events::KeyPressed& keyPressEvent)
    {
        return false;
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_scene()
    {
        R_PROFILE_FUNCTION();

        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        setup_lights();
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_camera()
    {
        R_PROFILE_FUNCTION();

        float viewport_width = (float)m_window->get_width();
        float viewport_height = (float)m_window->get_height();

        rex::AspectRatio aspect_ratio = rex::AspectRatio(viewport_width, viewport_height);

        float near_plane = lattice_rendering::camera_settings::NEAR_PLANE;
        float far_plane = lattice_rendering::camera_settings::FAR_PLANE;
        float fov = lattice_rendering::camera_settings::FIELD_OF_VIEW;

        m_camera.set_perspective(rex::FieldOfView(rex::DegAngle(fov), aspect_ratio.get_ratio()), rex::ClippingPlanes(near_plane, far_plane));
        m_camera.activate();

        rex::ecs::Entity camera_entity = m_scene->create_entity("camera"_sid);

        rex::ecs::TransformComponent& transform_component = camera_entity.get_component<rex::ecs::TransformComponent>();
        rex::ecs::CameraComponent& camera_component = camera_entity.add_component<rex::ecs::CameraComponent>(m_camera);

        m_camera_controller.set_camera(&camera_component.camera);
        m_camera_controller.set_camera_transform(&transform_component.transform);
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_scene_renderer()
    {
        R_PROFILE_FUNCTION();

        rex::SceneRenderPasses renderpasses;

        auto distance_eval = create_distance_evaluation_pass(lattice_rendering::create_lattice_options(), lattice_rendering::create_distance_evaluation_pass_options());
        auto composite = create_composite_pass(lattice_rendering::create_composite_pass_options());

        renderpasses.push_back(std::move(distance_eval));
        renderpasses.push_back(std::move(composite));

        m_scene_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(renderpasses));
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_lights()
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
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_distance_evaluation_pass(const rex::LatticeOptions& latticeOptions, const rex::DistanceEvaluationsPassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::LatticeDistanceEvaluationPass>(latticeOptions, options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        R_PROFILE_FUNCTION();

        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina