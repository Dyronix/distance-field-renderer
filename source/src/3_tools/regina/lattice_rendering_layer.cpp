#include "regina_pch.h"

#include "lattice_rendering_layer.h"

#include "ecs/components/camera_component.h"
#include "ecs/components/directional_light_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/tag_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/entity.h"
#include "ecs/scene.h"

#include "renderpasses/clear_pass.h"
#include "renderpasses/composite_pass.h"
#include "renderpasses/deferred_light_pass.h"
#include "renderpasses/deferred_light_visualization_pass.h"
#include "renderpasses/lattice_distance_evaluation_pass.h"
#include "renderpasses/heatmap_distance_evaluation_pass.h"

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

#include "mesh_factory.h"

#include "texture_importer.h"

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
            TIGER
        };

        using VolumeTypes = std::vector<VolumeType>;
        using VolumeNameMap = std::unordered_map<VolumeType, rex::StringID>;
        using VolumeLattice = rex::YesNoEnum;

        LatticeRenderingLayerDescription LAYER_DESCRIPTION;

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
        //-------------------------------------------------------------------------
        VolumeType get_active_volume_type()
        {
            return (VolumeType)LAYER_DESCRIPTION.volume_type;
        }

        // Render pass settings
        int32 MIN_NR_LIGHTS = 1;
        int32 MAX_NR_LIGHTS = 32;

        const rex::StringID DISTANCEEVALUATIONSPASS_NAME = "DistanceEvaluationsPass"_sid;
        const rex::StringID HEATMAPDISTANCEEVALUATIONSPASS_NAME = "HeatMapDistanceEvaluationsPass"_sid;
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

            float MIN_FOCUS_DISTANCE = 0.02f;
            float MAX_FOCUS_DISTANCE = 100.0f;
            float FOCUS_DISTANCE = 1.0;

            float ROTATION_SPEED = 1.0f;
            float MIN_PITCH_ANGLE = -50.0f;
            float MAX_PITCH_ANGLE = 50.0f;

            float MOVE_SENSITIVITY = 2.0f;
            float SCROLL_SENSITIVITY = 10.0f;
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
        rex::DistanceEvaluationsPassOptions create_distance_evaluation_pass_options(const rex::StringID& passName, const rex::StringID& shaderName)
        {
            rex::DistanceEvaluationsPassOptions options;

            options.pass_name = passName;
            options.shader_name = shaderName;

            options.sphere_tracer_options.max_iterations = LAYER_DESCRIPTION.max_iterations;
            options.sphere_tracer_options.max_march_distance = LAYER_DESCRIPTION.max_marching_distance;
            options.sphere_tracer_options.min_surface_distance = LAYER_DESCRIPTION.min_marching_distance; 

            const Lattice& lattice = lattice_library::get_lattice(get_volume_name_map()[get_active_volume_type()]);
            const LatticeMeta& lattice_meta = lattice.get_lattice_meta();

            const float scene_scale = get_volume_scale_map()[get_active_volume_type()];
            const float scene_offset = get_volume_offset_map()[get_active_volume_type()];

            options.sdf_scene_options.scene_scale = scene_scale;
            options.sdf_scene_options.scene_offset = scene_offset - LAYER_DESCRIPTION.min_marching_distance;
            options.sdf_scene_options.scene_size = calculate_scene_size(lattice) * 0.5f;
            options.sdf_scene_options.scene_center = rex::vec3(0.0f, 0.0f, 0.0f);

            options.sdf_scene_options.scene_voxel_grid_min_bounds = lattice_meta.voxel_grid_bounds.minimum;
            options.sdf_scene_options.scene_voxel_grid_max_bounds = lattice_meta.voxel_grid_bounds.maximum;
            options.sdf_scene_options.scene_voxel_grid_size = lattice_meta.voxel_grid_size;
            options.sdf_scene_options.scene_voxel_grid_cell_size = lattice_meta.voxel_grid_cell_size;

            options.sdf_scene_options.scene_data = volume_library::get_volume_data(get_volume_name_map()[get_active_volume_type()]);

            return options;
        }

        //-------------------------------------------------------------------------
        LatticeOptions create_lattice_options()
        {
            const Lattice& lattice = lattice_library::get_lattice(get_volume_name_map()[get_active_volume_type()]);
            const LatticeMeta& lattice_meta = lattice.get_lattice_meta();

            LatticeOptions options;

            options.lattice_strud_thickness = lattice_meta.lattice_strud_thickness;
            options.lattice_grid_size = lattice_meta.lattice_grid_size;
            options.lattice_grid_cell_size = lattice_meta.lattice_grid_cell_size;
            options.lattice_grid_min_bounds = lattice_meta.voxel_grid_bounds.minimum;
            options.lattice_grid_max_bounds = lattice_meta.voxel_grid_bounds.maximum;
            options.lattice_data_name = get_volume_name_map()[get_active_volume_type()];

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightPassOptions create_deferred_light_pass_options()
        {
            rex::DeferredLightPassOptions options;

            options.pass_name = lattice_rendering::DEFERREDLIGHTPASS_NAME;
            options.shader_name = "deferred_shading_lighting"_sid;
            options.g_position_buffer = {lattice_rendering::DISTANCEEVALUATIONSPASS_NAME, 0};
            options.g_normal_buffer = {lattice_rendering::DISTANCEEVALUATIONSPASS_NAME, 1};
            options.g_albedo_spec_buffer = {lattice_rendering::DISTANCEEVALUATIONSPASS_NAME, 2};

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightVisualizationPassOptions create_deferred_light_visualization_pass_options()
        {
            rex::DeferredLightVisualizationPassOptions options;

            options.pass_name = lattice_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME;
            options.color_pass_name = lattice_rendering::DEFERREDLIGHTPASS_NAME;
            options.depth_pass_name = lattice_rendering::DISTANCEEVALUATIONSPASS_NAME;
            options.shader_name = "deferred_shading_lighting_visualization"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options(const rex::StringID& colorBuffer, rex::ApplyGammaCorrection applyGamma, rex::ApplyToneMapping applyTone)
        {
            rex::CompositePassOptions options;

            options.pass_name = lattice_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = colorBuffer;
            options.apply_gamma_correction = applyGamma;
            options.apply_tone_mapping = applyTone;

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
            R_PROFILE_SCOPE("Load Shaders");

            load_shader("blit"_sid, "1000"_sid, "content\\shaders\\blit.vertex"_sid, "content\\shaders\\blit.fragment"_sid);
            load_shader("lattice"_sid, "1000"_sid, "content\\shaders\\lattice.vertex"_sid, "content\\shaders\\lattice.fragment"_sid);
            load_shader("deferred_shading_lighting"_sid, "1000"_sid, "content\\shaders\\deferred_shading_lighting.vertex"_sid, "content\\shaders\\deferred_shading_lighting.fragment"_sid);
            load_shader("deferred_shading_lighting_visualization"_sid, "1000"_sid, "content\\shaders\\deferred_shading_lighting_visualization.vertex"_sid, "content\\shaders\\deferred_shading_lighting_visualization.fragment"_sid);
            load_shader("heatmap"_sid, "1000"_sid, "content\\shaders\\heatmap.vertex"_sid, "content\\shaders\\heatmap.fragment"_sid);
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

            R_INFO("[TEXTURE] Import completed: {0}", name.to_string());
        }
        //-------------------------------------------------------------------------
        void load_textures()
        {
            load_texture("color_ramp", "content\\textures\\color_ramp.png", SRGB::NO, rex::Texture::Usage::UNSPECIFIED);
        }
        //-------------------------------------------------------------------------
        bool load_lattice(const rex::StringID& name, const rex::StringID& latticeMetaPath, const rex::StringID& latticeDataPath)
        {
            Lattice lattice = lattice_importer::import(name, latticeMetaPath, latticeDataPath);

            if (lattice.get_lattice_data().get_size() == 0)
            {
                R_ERROR("[LATTICE] Lattice with name: {0}, was not imported correctly", lattice.get_name().to_string());
                R_ERROR("[LATTICE] Import failed: {0}", lattice.get_name().to_string());
                return false;
            }

            lattice_library::add(std::move(lattice));

            R_INFO("[LATTICE] Import completed: {0}", name.to_string());
            
            return true;
        }
        //-------------------------------------------------------------------------
        bool load_lattice(const rex::StringID& sourceLocation, VolumeType volumeType)
        {
            rex::StringID source_location = sourceLocation.is_none() ? rex::create_sid("content\\lattices\\") : sourceLocation;

            std::stringstream lattice_stream;

            lattice_stream << source_location.to_string();
            lattice_stream << "\\";
            lattice_stream << get_volume_name_map()[volumeType];

            std::stringstream lattice_meta_path;
            lattice_meta_path << lattice_stream.str();
            lattice_meta_path << ".lattice.meta";

            std::stringstream lattice_path;
            lattice_path << lattice_stream.str();
            lattice_path << ".lattice";

            R_INFO("[VOLUME] Volume Meta Path: {0}", lattice_meta_path.str());
            R_INFO("[VOLUME] Volume Path: {0}", lattice_path.str());

            return load_lattice(get_volume_name_map()[volumeType], rex::create_sid(lattice_meta_path.str()), rex::create_sid(lattice_path.str()));
        }
        //-------------------------------------------------------------------------
        std::vector<VolumeType> load_lattices()
        {          
            load_lattice(get_volume_name_map()[VolumeType::BUNNY], "content\\lattices\\bunny.lattice.meta"_sid, "content\\lattices\\bunny.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::TIGER], "content\\lattices\\tiger.lattice.meta"_sid, "content\\lattices\\tiger.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::TORUS], "content\\lattices\\torus.lattice.meta"_sid, "content\\lattices\\torus.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::DRAGON], "content\\lattices\\dragon.lattice.meta"_sid, "content\\lattices\\dragon.lattice"_sid);
            load_lattice(get_volume_name_map()[VolumeType::MONKEY], "content\\lattices\\monkey.lattice.meta"_sid, "content\\lattices\\monkey.lattice"_sid);

            return {VolumeType::BUNNY, VolumeType::TIGER, VolumeType::TORUS, VolumeType::DRAGON, VolumeType::MONKEY};
        }
        //-------------------------------------------------------------------------
        std::vector<VolumeType> load_lattices(const LatticeRenderingLayerDescription& description)
        {
            R_PROFILE_SCOPE("Load Lattices")

            std::vector<VolumeType> loaded_lattice_types;

            if (!description.lattice_source_content_location.is_none())
            {
                if (load_lattice(description.lattice_source_content_location, (VolumeType)description.volume_type))
                {
                    loaded_lattice_types.push_back((VolumeType)description.volume_type);
                }
            }

            return loaded_lattice_types;
        }
        
        //-------------------------------------------------------------------------
        void load_primitive_geometry()
        {
            rex::mesh_factory::load();
        }
        
        //-------------------------------------------------------------------------
        bool load_volume(const rex::StringID& name, const rex::StringID& volumeMetaPath, const rex::StringID& volumeDataPath)
        {           
            Volume volume = volume_importer::import(name, volumeMetaPath, volumeDataPath);

            if (volume.get_volume_data().get_size() == 0)
            {
                R_ERROR("[VOLUME] Volume with name: {0}, was not imported correctly", volume.get_name().to_string());
                R_ERROR("[VOLUME] Import failed: {0}", volume.get_name().to_string());
                return false;
            }

            volume_library::add(std::move(volume));

            R_INFO("[VOLUME] Import completed: {0}", name.to_string());

            return true;
        }
        //-------------------------------------------------------------------------
        bool load_volume(const rex::StringID& sourceLocation, VolumeType volumeType)
        {
            rex::StringID source_location = sourceLocation.is_none() ? rex::create_sid("content\\volumes\\") : sourceLocation;

            std::stringstream volume_stream;

            volume_stream << source_location.to_string();
            volume_stream << "\\";
            volume_stream << get_volume_name_map()[volumeType];

            std::stringstream volume_meta_path;
            volume_meta_path << volume_stream.str();
            volume_meta_path << ".sdf.meta";

            std::stringstream volume_path;
            volume_path << volume_stream.str();
            volume_path << ".sdf";

            R_INFO("[VOLUME] Volume Meta Path: {0}", volume_meta_path.str());
            R_INFO("[VOLUME] Volume Path: {0}", volume_path.str());

            return load_volume(get_volume_name_map()[volumeType], rex::create_sid(volume_meta_path.str()), rex::create_sid(volume_path.str()));
        }
        //-------------------------------------------------------------------------
        std::vector<VolumeType> load_volumes()
        {
            R_PROFILE_SCOPE("Load Volumes")

            load_volume(get_volume_name_map()[VolumeType::BUNNY], "content\\volumes\\default\\bunny.sdf.meta"_sid, "content\\volumes\\default\\bunny.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::TIGER], "content\\volumes\\default\\tiger.sdf.meta"_sid, "content\\volumes\\default\\tiger.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::TORUS], "content\\volumes\\default\\torus.sdf.meta"_sid, "content\\volumes\\default\\torus.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::DRAGON], "content\\volumes\\default\\dragon.sdf.meta"_sid, "content\\volumes\\default\\dragon.sdf"_sid);
            load_volume(get_volume_name_map()[VolumeType::MONKEY], "content\\volumes\\default\\monkey.sdf.meta"_sid, "content\\volumes\\default\\monkey.sdf"_sid);

            return {VolumeType::BUNNY, VolumeType::TIGER, VolumeType::TORUS, VolumeType::DRAGON, VolumeType::MONKEY};
        }
        //-------------------------------------------------------------------------
        std::vector<VolumeType> load_volumes(const LatticeRenderingLayerDescription& description)
        {
            R_PROFILE_SCOPE("Load Volumes")

            std::vector<VolumeType> loaded_volume_types;

            if (!description.volume_source_content_location.is_none())
            {
                if (load_volume(description.volume_source_content_location, (VolumeType)description.volume_type))
                {
                    loaded_volume_types.push_back((VolumeType)description.volume_type);
                }
            }

            return loaded_volume_types;
        }
    } // namespace lattice_rendering

    //-------------------------------------------------------------------------
    LatticeRenderingLayer::LatticeRenderingLayer(const rex::CoreWindow* window, const LatticeRenderingLayerDescription& description)
        : Layer("regina_layer"_sid, -1, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, lattice_rendering::create_orbit_camera_description())
        , m_window(window)
        , m_active_renderer(nullptr)
    {
        lattice_rendering::LAYER_DESCRIPTION = description;
    }
    //-------------------------------------------------------------------------
    LatticeRenderingLayer::~LatticeRenderingLayer()
    {
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_attach()
    {
        lattice_rendering::load_textures();
        lattice_rendering::load_volumes(lattice_rendering::LAYER_DESCRIPTION);
        lattice_rendering::load_lattices(lattice_rendering::LAYER_DESCRIPTION);
        lattice_rendering::load_shaders();
        lattice_rendering::load_primitive_geometry();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_detach()
    {
        rex::mesh_factory::clear();
        rex::shader_library::clear();

        lattice_library::clear();
        volume_library::clear();

        rex::texture_library::clear();

        rex::FrameBufferPool::instance()->clear();
        rex::UniformBufferSet::instance()->clear();

        m_sdf_renderer->destroy();
        m_sdf_renderer.reset();

        if (m_heatmap_renderer)
        {
            m_heatmap_renderer->destroy();
            m_heatmap_renderer.reset();
        }

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_update(const rex::FrameInfo& info)
    {
        if (lattice_rendering::LAYER_DESCRIPTION.animate)
        {
            animate_camera(info);
        }        

        m_camera_controller.on_update(info);

        m_scene->update();

        if (m_active_renderer != nullptr)
        {
            m_active_renderer->set_viewport_width(m_window->get_width());
            m_active_renderer->set_viewport_height(m_window->get_height());
            m_active_renderer->begin_scene();
            m_active_renderer->end_scene();
        }
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::on_event(rex::events::Event& event)
    {       
        m_camera_controller.on_event(event);

        rex::events::EventDispatcher dispatcher(event);

        dispatcher.dispatch<rex::events::KeyPressed>([&](rex::events::KeyPressed& pressedEvt) { return on_key_pressed(pressedEvt); });
    }

    //-------------------------------------------------------------------------
    bool LatticeRenderingLayer::on_key_pressed(const rex::events::KeyPressed& keyPressEvent)
    {
        switch (keyPressEvent.get_key())
        {
            case R_KEY_F3: toggle_camera_animation(); return true;

            case R_KEY_W: increment_sdf_scale(); return true;
            case R_KEY_A: increment_sdf_offset(); return true;
            case R_KEY_S: decrement_sdf_scale(); return true;
            case R_KEY_D: decrement_sdf_offset(); return true;
            case R_KEY_TAB:
            {
                if (lattice_rendering::LAYER_DESCRIPTION.use_heatmap)
                {
                    if (m_active_renderer == m_sdf_renderer.get())
                        switch_to_heatmap();
                    else
                        switch_to_sdf();
                    return true;
                }

                return false;
            }
            default: return false;
        }
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::animate_camera(const rex::FrameInfo& info)
    {
        float focus_distance_speed = 0.5f;
        float current_focus_distance = m_camera_controller.get_focus_distance();

        current_focus_distance += focus_distance_speed * info.delta_time.to_seconds();

        m_camera_controller.set_focus_distance(current_focus_distance);

        m_camera_controller.orbit(rex::vec2(-0.01f, 0.3f));
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::toggle_camera_animation()
    {
        lattice_rendering::LAYER_DESCRIPTION.animate = !lattice_rendering::LAYER_DESCRIPTION.animate;
    }

        //-------------------------------------------------------------------------
    void LatticeRenderingLayer::decrement_sdf_scale()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(lattice_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
            lattice_rendering::VolumeType active_volume_type = lattice_rendering::get_active_volume_type();

            const Lattice& lattice = lattice_library::get_lattice(lattice_rendering::get_volume_name_map()[active_volume_type]);

            scene_size = lattice_rendering::calculate_scene_size(lattice);
            scene_size = scene_size * scene_scale;
        }

        sdf_scene_options.scene_scale = scene_scale;
        sdf_scene_options.scene_size = scene_size;

        R_INFO("Scale SDF, scaling down SDF: {0}", sdf_scene_options.scene_scale);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::decrement_sdf_offset()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(lattice_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
    void LatticeRenderingLayer::increment_sdf_scale()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(lattice_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
            lattice_rendering::VolumeType active_volume_type = lattice_rendering::get_active_volume_type();

            const Lattice& lattice = lattice_library::get_lattice(lattice_rendering::get_volume_name_map()[active_volume_type]);

            scene_size = lattice_rendering::calculate_scene_size(lattice);
            scene_size = scene_size * scene_scale;
        }

        sdf_scene_options.scene_scale = scene_scale;
        sdf_scene_options.scene_size = scene_size;

        R_INFO("Scale SDF, scaling up SDF: {0}", sdf_scene_options.scene_scale);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::increment_sdf_offset()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(lattice_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
    void LatticeRenderingLayer::switch_to_heatmap()
    {
        m_active_renderer = m_heatmap_renderer.get();
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::switch_to_sdf()
    {
        m_active_renderer = m_sdf_renderer.get();
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_scene()
    {
        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        setup_lights();
    }
    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_camera()
    {
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
        setup_sdf_renderer();
        if (lattice_rendering::LAYER_DESCRIPTION.use_heatmap)
        {
            setup_heatmap_renderer();
        }

        m_active_renderer = m_sdf_renderer.get();
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_sdf_renderer()
    {
        rex::SceneRenderPasses sdf_renderpasses;

        auto distance_eval = create_distance_evaluation_pass(lattice_rendering::create_lattice_options(), lattice_rendering::create_distance_evaluation_pass_options(lattice_rendering::DISTANCEEVALUATIONSPASS_NAME, "lattice"_sid));
        auto deferred_light = create_deferred_light_pass(lattice_rendering::create_deferred_light_pass_options());
        auto deferred_light_visualization = create_deferred_light_visualization_pass(lattice_rendering::create_deferred_light_visualization_pass_options());
        auto sdf_composite = create_composite_pass(lattice_rendering::create_composite_pass_options(lattice_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME, rex::ApplyGammaCorrection::YES, rex::ApplyToneMapping::YES));

        sdf_renderpasses.push_back(std::move(distance_eval));
        sdf_renderpasses.push_back(std::move(deferred_light));
        sdf_renderpasses.push_back(std::move(deferred_light_visualization));
        sdf_renderpasses.push_back(std::move(sdf_composite));

        m_sdf_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(sdf_renderpasses));
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_heatmap_renderer()
    {
        rex::SceneRenderPasses heatmap_renderpasses;

        auto heatmap = create_heatmap_distance_evaluation_pass(lattice_rendering::create_distance_evaluation_pass_options(lattice_rendering::HEATMAPDISTANCEEVALUATIONSPASS_NAME, "heatmap"_sid));
        auto heatmap_composite = create_composite_pass(lattice_rendering::create_composite_pass_options(lattice_rendering::HEATMAPDISTANCEEVALUATIONSPASS_NAME, rex::ApplyGammaCorrection::NO, rex::ApplyToneMapping::NO));

        heatmap_renderpasses.push_back(std::move(heatmap));
        heatmap_renderpasses.push_back(std::move(heatmap_composite));

        m_heatmap_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(heatmap_renderpasses));
    }

    //-------------------------------------------------------------------------
    void LatticeRenderingLayer::setup_lights()
    {
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
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_distance_evaluation_pass(const LatticeOptions& latticeOptions, const rex::DistanceEvaluationsPassOptions& options) const
    {
        return std::make_unique<LatticeDistanceEvaluationPass>(latticeOptions, options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_heatmap_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const
    {
        return std::make_unique<rex::HeatMapDistanceEvaluationPass>("color_ramp"_sid, options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const
    {
        return std::make_unique<rex::DeferredLightPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const
    {
        return std::make_unique<rex::DeferredLightVisualizationPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> LatticeRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {       
        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina