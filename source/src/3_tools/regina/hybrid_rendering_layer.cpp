#include "regina_pch.h"

#include "hybrid_rendering_layer.h" 

#include "ecs/components/camera_component.h"
#include "ecs/components/directional_light_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/tag_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/entity.h"
#include "ecs/scene.h"

#include "renderpasses/clear_pass.h"
#include "renderpasses/composite_pass.h"
#include "renderpasses/distance_evaluation_pass.h"
#include "renderpasses/heatmap_distance_evaluation_pass.h"
#include "renderpasses/deferred_light_pass.h"
#include "renderpasses/deferred_light_visualization_pass.h"
#include "renderpasses/deferred_geometry_pass.h"

#include "resources/frame_buffer_pool.h"
#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/uniform_buffer_set.h"

#include "renderer/renderer.h"

#include "mesh_factory.h"

#include "model_library.h"
#include "model_importer.h"
#include "model.h"

#include "volume_importer.h"
#include "volume_library.h"

#include "texture_importer.h"

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
    namespace hybrid_rendering
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

            CROSS_CUBE_RIBS,
            CROSS,
            CUBE_RIBS,
            DOUBLE_TETRA_OCTA_RIBS,
            DOUBLE_TETRA_RIBS,
            FKA,
            OCTAHEDRON_RIB,
            OECHS,
            TETRAHEDRON_RIBS
        };

        using VolumeTypes = std::vector<VolumeType>;
        using VolumeNameMap = std::unordered_map<VolumeType, rex::StringID>;
        using VolumeLattice = rex::YesNoEnum;

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
            
            CROSS_CUBE_RIBS,
            CROSS,
            CUBE_RIBS,
            DOUBLE_TETRA_OCTA_RIBS,
            DOUBLE_TETRA_RIBS,
            FKA,
            OCTAHEDRON_RIB,
            OECHS,
            TETRAHEDRON_RIBS
        };

        using MeshTypes = std::vector<MeshType>;
        using MeshNameMap = std::unordered_map<MeshType, rex::StringID>;
        using MeshScaleMap = std::unordered_map<MeshType, rex::vec3>;

        HybridRenderingLayerDescription LAYER_DESCRIPTION;

        //-------------------------------------------------------------------------
        VolumeNameMap& get_volume_name_map()
        {
            static VolumeNameMap VOLUME_NAME_MAP =
            {
                { VolumeType::BUNNY,      "bunny"_sid },
                { VolumeType::CUBE,       "cube"_sid },
                { VolumeType::CYLINDER,   "cylinder"_sid },
                { VolumeType::MONKEY,     "monkey"_sid },
                { VolumeType::SPHERE,     "sphere"_sid },
                { VolumeType::TORUS,      "torus"_sid },
                { VolumeType::DRAGON,     "dragon"_sid },
                { VolumeType::TIGER,      "tiger"_sid },

                { VolumeType::CROSS_CUBE_RIBS,        "cross_cube_ribs"_sid},
                { VolumeType::CROSS,                  "cross"_sid },
                { VolumeType::CUBE_RIBS,              "cube_ribs"_sid },
                { VolumeType::DOUBLE_TETRA_OCTA_RIBS, "double_tetra_octa_ribs"_sid },
                { VolumeType::DOUBLE_TETRA_RIBS,      "double_tetra_ribs"_sid },
                { VolumeType::FKA,                    "fka"_sid},
                { VolumeType::OCTAHEDRON_RIB,         "octahedron_rib"_sid},
                { VolumeType::OECHS,                  "oechs"_sid},
                { VolumeType::TETRAHEDRON_RIBS,       "tetraheron_ribs"_sid},
            };

            return VOLUME_NAME_MAP;
        }
        //-------------------------------------------------------------------------
        std::unordered_map<VolumeType, float>& get_volume_scale_map()
        {
            static std::unordered_map<VolumeType, float> MAP
            {
                { VolumeType::BUNNY,      7.0f},
                { VolumeType::CUBE,       1.0f},
                { VolumeType::CYLINDER,   1.0f},
                { VolumeType::MONKEY,     1.0f},
                { VolumeType::SPHERE,     1.0f},
                { VolumeType::TORUS,      1.0f},
                { VolumeType::DRAGON,     0.02f},
                { VolumeType::TIGER,      2.0f},

                { VolumeType::CROSS_CUBE_RIBS,        1.0f},
                { VolumeType::CROSS,                  1.0f},
                { VolumeType::CUBE_RIBS,              1.0f},
                { VolumeType::DOUBLE_TETRA_OCTA_RIBS, 1.0f},
                { VolumeType::DOUBLE_TETRA_RIBS,      1.0f},
                { VolumeType::FKA,                    1.0f},
                { VolumeType::OCTAHEDRON_RIB,         1.0f},
                { VolumeType::OECHS,                  1.0f},
                { VolumeType::TETRAHEDRON_RIBS,       1.0f}
            };

            return MAP;
        }
        //-------------------------------------------------------------------------
        std::unordered_map<VolumeType, float>& get_volume_offset_map()
        {
            static std::unordered_map<VolumeType, float> MAP
            {
                { VolumeType::BUNNY,      -0.001f},
                { VolumeType::CUBE,       -0.018f},
                { VolumeType::CYLINDER,   -0.018f},
                { VolumeType::MONKEY,     -0.003f},
                { VolumeType::SPHERE,     -0.002f},
                { VolumeType::TORUS,      -0.004f},
                { VolumeType::DRAGON,     -0.003f},
                { VolumeType::TIGER,      -0.003f},

                { VolumeType::CROSS_CUBE_RIBS,        -0.002f},
                { VolumeType::CROSS,                  -0.002f},
                { VolumeType::CUBE_RIBS,              -0.002f},
                { VolumeType::DOUBLE_TETRA_OCTA_RIBS, -0.002f},
                { VolumeType::DOUBLE_TETRA_RIBS,      -0.002f},
                { VolumeType::FKA,                    -0.002f},
                { VolumeType::OCTAHEDRON_RIB,         -0.002f},
                { VolumeType::OECHS,                  -0.002f},
                { VolumeType::TETRAHEDRON_RIBS,       -0.002f}
            };

            return MAP;
        }
        //-------------------------------------------------------------------------
        VolumeType get_active_volume_type()
        {
            return (VolumeType)LAYER_DESCRIPTION.volume_type;
        }

        //-------------------------------------------------------------------------
        MeshNameMap& get_mesh_name_map()
        {
            static MeshNameMap s_mesh_name_map =
            {
                { MeshType::BUNNY,      "bunny"_sid },
                { MeshType::CUBE,       "cube"_sid },
                { MeshType::CYLINDER,   "cylinder"_sid },
                { MeshType::MONKEY,     "monkey"_sid },
                { MeshType::SPHERE,     "sphere"_sid },
                { MeshType::TORUS,      "torus"_sid },
                { MeshType::DRAGON,     "dragon"_sid },
                { MeshType::TIGER,      "tiger"_sid },
                
                { MeshType::CROSS_CUBE_RIBS,        "cross_cube_ribs"_sid},
                { MeshType::CROSS,                  "cross"_sid },
                { MeshType::CUBE_RIBS,              "cube_ribs"_sid },
                { MeshType::DOUBLE_TETRA_OCTA_RIBS, "double_tetra_octa_ribs"_sid },
                { MeshType::DOUBLE_TETRA_RIBS,      "double_tetra_ribs"_sid },
                { MeshType::FKA,                    "fka"_sid},
                { MeshType::OCTAHEDRON_RIB,         "octahedron_rib"_sid},
                { MeshType::OECHS,                  "oechs"_sid},
                { MeshType::TETRAHEDRON_RIBS,       "tetraheron_ribs"_sid},
            };

            return s_mesh_name_map;
        }
        //-------------------------------------------------------------------------
        MeshScaleMap& get_mesh_scale_map()
        {
            static MeshScaleMap s_mesh_scale_map
            {
                { MeshType::BUNNY,      {10.0, 10.0, 10.0}},
                { MeshType::CUBE,       {0.5, 0.5, 0.5}},
                { MeshType::CYLINDER,   {0.5, 0.5, 0.5}},
                { MeshType::MONKEY,     {0.5, 0.5, 0.5} },
                { MeshType::SPHERE,     {0.5, 0.5, 0.5} },
                { MeshType::TORUS,      {0.5, 0.5, 0.5} },
                { MeshType::DRAGON,     {0.02, 0.02, 0.02} },
                { MeshType::TIGER,      {1.5, 1.5, 1.5} },

                { MeshType::CROSS_CUBE_RIBS,        {0.5, 0.5, 0.5}},
                { MeshType::CROSS,                  {0.5, 0.5, 0.5}},
                { MeshType::CUBE_RIBS,              {0.5, 0.5, 0.5}},
                { MeshType::DOUBLE_TETRA_OCTA_RIBS, {0.5, 0.5, 0.5}},
                { MeshType::DOUBLE_TETRA_RIBS,      {0.5, 0.5, 0.5}},
                { MeshType::FKA,                    {0.5, 0.5, 0.5}},
                { MeshType::OCTAHEDRON_RIB,         {0.5, 0.5, 0.5}},
                { MeshType::OECHS,                  {0.5, 0.5, 0.5}},
                { MeshType::TETRAHEDRON_RIBS,       {0.5, 0.5, 0.5}},
            };

            return s_mesh_scale_map;
        }
        //-------------------------------------------------------------------------
        MeshType get_active_mesh_type()
        {
            return (MeshType)LAYER_DESCRIPTION.mesh_type;
        }

        // Render pass settings
        int32 MIN_NR_LIGHTS = 1;
        int32 MAX_NR_LIGHTS = 32;

        const rex::StringID DEFERREDGEOMETRYPASS_NAME = "DeferredGeometryPass"_sid;
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
            
            float MAX_MARCH_DISTANCE = 100.0f;
            float MIN_SURFACE_DISTANCE = 0.01f;
        } // namespace renderpass_settings

        //-------------------------------------------------------------------------
        rex::vec3 calculate_scene_size(const rex::AABB& voxelGridBounds)
        {
            rex::vec3 voxel_grid_size = voxelGridBounds.maximum - voxelGridBounds.minimum;

            float longest_edge = rex::max_coeff(voxel_grid_size);

            float x_width = rex::remap(voxel_grid_size.x, 0.0f, 0.0f, longest_edge, 1.0f);
            float y_width = rex::remap(voxel_grid_size.y, 0.0f, 0.0f, longest_edge, 1.0f);
            float z_width = rex::remap(voxel_grid_size.z, 0.0f, 0.0f, longest_edge, 1.0f);

            return rex::vec3(x_width, y_width, z_width);
        }

        //-------------------------------------------------------------------------
        rex::DeferredGeometryPassOptions create_deferred_geometry_pass_options()
        {          
            rex::DeferredGeometryPassOptions options;

            options.pass_name = hybrid_rendering::DEFERREDGEOMETRYPASS_NAME;
            options.backface_culling = hybrid_rendering::renderpass_settings::BACKFACE_CULLING;
            options.geometry_shader_name = "g_buffer"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DistanceEvaluationsPassOptions create_distance_evaluation_pass_options(const rex::StringID& passName, const rex::StringID& shaderName)
        {
            rex::DistanceEvaluationsPassOptions options;

            options.pass_name = passName;
            options.shader_name = shaderName;

            options.sphere_tracer_options.max_iterations = LAYER_DESCRIPTION.max_iterations;
            options.sphere_tracer_options.max_march_distance = renderpass_settings::MAX_MARCH_DISTANCE;
            options.sphere_tracer_options.min_surface_distance = renderpass_settings::MIN_SURFACE_DISTANCE;

            VolumeType active_volume_type = get_active_volume_type();

            const Volume& volume = volume_library::get_volume(get_volume_name_map()[active_volume_type]);
            const VolumeMeta& volume_meta = volume.get_volume_meta();
          
            const float scene_scale = get_volume_scale_map()[active_volume_type];
            const float scene_offset = get_volume_offset_map()[active_volume_type];

            options.sdf_scene_options.scene_scale = scene_scale;
            options.sdf_scene_options.scene_offset = scene_offset - renderpass_settings::MIN_SURFACE_DISTANCE;
            options.sdf_scene_options.scene_size = calculate_scene_size(volume.get_voxel_grid_bounds()) * 0.5f;
            options.sdf_scene_options.scene_center = rex::vec3(0.0f, 0.0f, 0.0f);

            options.sdf_scene_options.scene_voxel_grid_min_bounds = volume_meta.voxel_grid_bounds.minimum;
            options.sdf_scene_options.scene_voxel_grid_max_bounds = volume_meta.voxel_grid_bounds.maximum;
            options.sdf_scene_options.scene_voxel_grid_size = volume_meta.voxel_grid_size;
            options.sdf_scene_options.scene_voxel_grid_cell_size = volume_meta.voxel_grid_cell_size;

            options.sdf_scene_options.scene_data = volume_library::get_volume_data(get_volume_name_map()[active_volume_type]);

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightPassOptions create_deferred_light_pass_options()
        {
            rex::DeferredLightPassOptions options;

            options.pass_name = hybrid_rendering::DEFERREDLIGHTPASS_NAME;
            options.shader_name = "deferred_shading_lighting"_sid;
            options.g_position_buffer = {hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME, 0};
            options.g_normal_buffer = {hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME, 1};
            options.g_albedo_spec_buffer = {hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME, 2};

            return options;
        }
        //-------------------------------------------------------------------------
        rex::DeferredLightVisualizationPassOptions create_deferred_light_visualization_pass_options()
        {
            rex::DeferredLightVisualizationPassOptions options;

            options.pass_name = hybrid_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME;
            options.color_pass_name = hybrid_rendering::DEFERREDLIGHTPASS_NAME;
            options.depth_pass_name = hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME;
            options.shader_name = "deferred_shading_lighting_visualization"_sid;

            return options;
        }
        //-------------------------------------------------------------------------
        rex::CompositePassOptions create_composite_pass_options(const rex::StringID& colorBuffer, rex::ApplyGammaCorrection applyGamma, rex::ApplyToneMapping applyTone)
        {
            rex::CompositePassOptions options;

            options.pass_name = hybrid_rendering::COMPOSITEPASS_NAME;
            options.shader_name = "blit"_sid;
            options.color_buffer = colorBuffer;
            options.gamma_correction = hybrid_rendering::renderpass_settings::GAMMA_CORRECTION;
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
            rex::vec3 camera_pos = hybrid_rendering::camera_settings::CAMERA_POSITION;
            rex::quaternion camera_rot = rex::quaternion(hybrid_rendering::camera_settings::CAMERA_ROTATION);

            bool can_rotate_pitch = hybrid_rendering::camera_settings::CAN_ROTATE_PITCH;
            bool can_rotate_yaw = hybrid_rendering::camera_settings::CAN_ROTATE_YAW;
            bool can_zoom = hybrid_rendering::camera_settings::CAN_ZOOM;

            bool initial_enabled = hybrid_rendering::camera_settings::IS_ENABLED;

            description.camera_settings.camera_position = camera_pos;
            description.camera_settings.camera_rotation = camera_rot;
            description.camera_settings.can_rotate_pitch = can_rotate_pitch ? CanRotatePitch::YES : CanRotatePitch::NO;
            description.camera_settings.can_rotate_yaw = can_rotate_yaw ? CanRotateYaw::YES : CanRotateYaw::NO;
            description.camera_settings.can_zoom = can_zoom ? CanZoom::YES : CanZoom::NO;
            description.camera_settings.enabled = initial_enabled;

            // Focus
            description.focus_settings = create_focus_settings(hybrid_rendering::camera_settings::CAMERA_FOCUS, hybrid_rendering::camera_settings::MIN_FOCUS_DISTANCE, hybrid_rendering::camera_settings::MAX_FOCUS_DISTANCE,
                                                               hybrid_rendering::camera_settings::FOCUS_DISTANCE);

            // Orbit
            description.orbit_settings = create_orbit_settings(hybrid_rendering::camera_settings::ROTATION_SPEED, hybrid_rendering::camera_settings::MIN_PITCH_ANGLE, hybrid_rendering::camera_settings::MAX_PITCH_ANGLE);

            // Mouse
            description.mouse_settings = create_mouse_settings(hybrid_rendering::camera_settings::MOVE_SENSITIVITY, hybrid_rendering::camera_settings::SCROLL_SENSITIVITY);

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
            load_shader("g_buffer_distance_field"_sid, "1000"_sid, "content\\shaders\\g_buffer_distance_field.vertex"_sid, "content\\shaders\\g_buffer_distance_field.fragment"_sid);
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
        void load_primitive_geometry()
        {
            rex::mesh_factory::load();
        }
        //-------------------------------------------------------------------------
        bool load_volume(const rex::StringID& name, const rex::StringID& volumeMetaPath, const rex::StringID& volumeDataPath)
        {
            Volume volume = volume_importer::import(name, volumeMetaPath, volumeDataPath);

            if(volume.get_volume_data().get_size() == 0)
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

            std::vector<VolumeType> loaded_volume_types;

            if (!LAYER_DESCRIPTION.source_content_location.is_none())
            {
                if (load_volume(LAYER_DESCRIPTION.source_content_location, (VolumeType)LAYER_DESCRIPTION.volume_type)) 
                {
                    loaded_volume_types.push_back((VolumeType)LAYER_DESCRIPTION.volume_type); 
                }
            }

            return loaded_volume_types;
        }
        //-------------------------------------------------------------------------
        bool load_custom_geometry(const rex::StringID& sourceLocation, MeshType meshType)
        {
            rex::StringID source_location = sourceLocation.is_none() ? rex::create_sid("content\\meshes\\") : sourceLocation;

            std::stringstream mesh_stream;

            mesh_stream << source_location.to_string();
            mesh_stream << "\\";
            mesh_stream << get_mesh_name_map()[meshType];

            std::stringstream mesh_path;
            mesh_path << mesh_stream.str();
            mesh_path << ".obj";

            R_INFO("[Model] Mesh Path: {0}", mesh_path.str());

            return rex::model_library::add(model_importer::import(rex::create_sid(mesh_path.str()), get_mesh_name_map()[meshType]));
        }
        //-------------------------------------------------------------------------
        std::vector<MeshType> load_custom_geometry()
        {
            std::vector<MeshType> loaded_mesh_types;

            if (!LAYER_DESCRIPTION.source_content_location.is_none())
            {
                load_custom_geometry(LAYER_DESCRIPTION.source_content_location, (MeshType)LAYER_DESCRIPTION.mesh_type);
            }

            return loaded_mesh_types;
        }
    } // namespace hybrid_rendering

    //-------------------------------------------------------------------------
    HybridRenderingLayer::HybridRenderingLayer(const rex::CoreWindow* window, const HybridRenderingLayerDescription& description)
        : Layer("regina_layer"_sid, -1, EnableImGUIRendering::NO)
        , m_camera_controller(rex::win32::Input::instance(), R_MOUSE_BUTTON_LEFT, hybrid_rendering::create_orbit_camera_description())
        , m_window(window)
        , m_active_renderer(nullptr)
    {
        hybrid_rendering::LAYER_DESCRIPTION = description;
    }
    //-------------------------------------------------------------------------
    HybridRenderingLayer::~HybridRenderingLayer()
    {
        
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::on_attach()
    {
        hybrid_rendering::load_textures();
        hybrid_rendering::load_shaders();
        hybrid_rendering::load_primitive_geometry();
        hybrid_rendering::load_custom_geometry();
        hybrid_rendering::load_volumes();

        setup_scene();
        setup_camera();
        setup_scene_renderer();
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::on_detach()
    {
        rex::model_library::clear();
        rex::mesh_factory::clear(); 
        rex::shader_library::clear();
        rex::texture_library::clear();

        volume_library::clear();

        rex::FrameBufferPool::instance()->clear();
        rex::UniformBufferSet::instance()->clear();

        m_offscreen_renderer->destroy();
        m_offscreen_renderer.reset();

        m_hybrid_renderer->destroy();
        m_hybrid_renderer.reset();

        if (m_heatmap_renderer)
        {
            m_heatmap_renderer->destroy();
            m_heatmap_renderer.reset();
        }

        m_scene.reset();
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::on_update(const rex::FrameInfo& info)
    {
        if (hybrid_rendering::LAYER_DESCRIPTION.animate)
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
    void HybridRenderingLayer::on_event(rex::events::Event& event)
    {
        m_camera_controller.on_event(event);

        rex::events::EventDispatcher dispatcher(event);

        dispatcher.dispatch<rex::events::KeyPressed>([&](rex::events::KeyPressed& pressedEvt) { return on_key_pressed(pressedEvt); });
    }

    //-------------------------------------------------------------------------
    bool HybridRenderingLayer::on_key_pressed(const rex::events::KeyPressed& keyPressEvent)
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
                if (hybrid_rendering::LAYER_DESCRIPTION.use_heatmap)
                {
                    if (m_active_renderer == m_hybrid_renderer.get())
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
    void HybridRenderingLayer::animate_camera(const rex::FrameInfo& info)
    {
        float focus_distance_speed = 0.5f;
        float current_focus_distance = m_camera_controller.get_focus_distance();

        current_focus_distance += focus_distance_speed * info.delta_time.to_seconds();

        m_camera_controller.set_focus_distance(current_focus_distance);

        m_camera_controller.orbit(rex::vec2(-0.01f, 0.3f));
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::toggle_camera_animation()
    {
        hybrid_rendering::LAYER_DESCRIPTION.animate = !hybrid_rendering::LAYER_DESCRIPTION.animate;
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::decrement_sdf_scale()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
            hybrid_rendering::VolumeType active_volume_type = hybrid_rendering::get_active_volume_type();

            const Volume& volume = volume_library::get_volume(hybrid_rendering::get_volume_name_map()[active_volume_type]);

            scene_size = hybrid_rendering::calculate_scene_size(volume.get_voxel_grid_bounds());
            scene_size = scene_size * scene_scale;
        }
        
        sdf_scene_options.scene_scale = scene_scale;
        sdf_scene_options.scene_size = scene_size;

        R_INFO("Scale SDF, scaling down SDF: {0}", sdf_scene_options.scene_scale);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::decrement_sdf_offset()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
    void HybridRenderingLayer::increment_sdf_scale()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
            hybrid_rendering::VolumeType active_volume_type = hybrid_rendering::get_active_volume_type();

            const Volume& volume = volume_library::get_volume(hybrid_rendering::get_volume_name_map()[active_volume_type]);

            scene_size = hybrid_rendering::calculate_scene_size(volume.get_voxel_grid_bounds());
            scene_size = scene_size * scene_scale;
        }

        sdf_scene_options.scene_scale = scene_scale;
        sdf_scene_options.scene_size = scene_size;

        R_INFO("Scale SDF, scaling up SDF: {0}", sdf_scene_options.scene_scale);

        distance_eval->set_sdf_scene_options(sdf_scene_options);
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::increment_sdf_offset()
    {
        if (m_active_renderer == nullptr)
        {
            return;
        }

        rex::SceneRenderPass* render_pass = m_active_renderer->get_scene_render_pass(hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME);
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
    void HybridRenderingLayer::switch_to_heatmap()
    {
        m_active_renderer = m_heatmap_renderer.get();
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::switch_to_sdf()
    {
        m_active_renderer = m_hybrid_renderer.get();
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_scene()
    {
        setup_hybrid_scene();
        setup_offscreen_scene();
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_hybrid_scene()
    {
        int32 viewport_width = m_window->get_width();
        int32 viewport_height = m_window->get_height();

        m_scene = rex::ecs::Scene::create_empty("regina"_sid, viewport_width, viewport_height);

        // Create grey material
        //
        m_material = rex::ResourceFactory::create_material(rex::shader_library::get("g_buffer"), "G Buffer Material"_sid);
        m_material->set_texture2d("u_Texture_Diffuse", rex::Renderer::get_white_texture());
        m_material->set_texture2d("u_Texture_Specular", rex::Renderer::get_black_texture());

        setup_lights();
        setup_geometry();
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_offscreen_scene()
    {
        // Offscreen scene impl ...
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_camera()
    {
        float viewport_width = (float)m_window->get_width();
        float viewport_height = (float)m_window->get_height();

        rex::AspectRatio aspect_ratio = rex::AspectRatio(viewport_width, viewport_height);

        float near_plane = hybrid_rendering::camera_settings::NEAR_PLANE;
        float far_plane = hybrid_rendering::camera_settings::FAR_PLANE;
        float fov = hybrid_rendering::camera_settings::FIELD_OF_VIEW;

        m_camera.set_perspective(rex::FieldOfView(rex::DegAngle(fov), aspect_ratio.get_ratio()), rex::ClippingPlanes(near_plane, far_plane));
        m_camera.activate();

        rex::ecs::Entity camera_entity = m_scene->create_entity("camera"_sid);

        rex::ecs::TransformComponent& transform_component = camera_entity.get_component<rex::ecs::TransformComponent>();
        rex::ecs::CameraComponent& camera_component = camera_entity.add_component<rex::ecs::CameraComponent>(m_camera);

        m_camera_controller.set_camera(&camera_component.camera);
        m_camera_controller.set_camera_transform(&transform_component.transform);
    }
    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_scene_renderer()
    {
        setup_hybrid_renderer();
        setup_offscreen_renderer();

        if (hybrid_rendering::LAYER_DESCRIPTION.use_heatmap)
        {
            setup_heatmap_renderer();
        }

        m_active_renderer = m_hybrid_renderer.get();
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_hybrid_renderer()
    {
        rex::SceneRenderPasses hybrid_renderpasses;

        auto distance_eval = create_distance_evaluation_pass(hybrid_rendering::create_distance_evaluation_pass_options(hybrid_rendering::DISTANCEEVALUATIONSPASS_NAME, "g_buffer_distance_field"_sid));
        auto deferred_geometry = create_deferred_geometry_pass(hybrid_rendering::create_deferred_geometry_pass_options());
        auto deferred_light = create_deferred_light_pass(hybrid_rendering::create_deferred_light_pass_options());
        auto deferred_light_visualization = create_deferred_light_visualization_pass(hybrid_rendering::create_deferred_light_visualization_pass_options());
        auto sdf_composite = create_composite_pass(hybrid_rendering::create_composite_pass_options(hybrid_rendering::DEFERREDLIGHTVISUALIZATIONPASS_NAME, rex::ApplyGammaCorrection::YES, rex::ApplyToneMapping::YES));

        hybrid_renderpasses.push_back(std::move(distance_eval));
        hybrid_renderpasses.push_back(std::move(deferred_geometry));
        hybrid_renderpasses.push_back(std::move(deferred_light));
        hybrid_renderpasses.push_back(std::move(deferred_light_visualization));
        hybrid_renderpasses.push_back(std::move(sdf_composite));

        m_hybrid_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(hybrid_renderpasses));
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_offscreen_renderer()
    {
        // Offscreen renderer impl ...
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_heatmap_renderer()
    {
        rex::SceneRenderPasses heatmap_renderpasses;

        auto heatmap = create_heatmap_distance_evaluation_pass(hybrid_rendering::create_distance_evaluation_pass_options(hybrid_rendering::HEATMAPDISTANCEEVALUATIONSPASS_NAME, "heatmap"_sid));
        auto heatmap_composite = create_composite_pass(hybrid_rendering::create_composite_pass_options(hybrid_rendering::HEATMAPDISTANCEEVALUATIONSPASS_NAME, rex::ApplyGammaCorrection::NO, rex::ApplyToneMapping::NO));

        heatmap_renderpasses.push_back(std::move(heatmap));
        heatmap_renderpasses.push_back(std::move(heatmap_composite));

        m_heatmap_renderer = rex::make_ref<rex::SceneRenderer>(m_scene, std::move(heatmap_renderpasses));
    }

    //-------------------------------------------------------------------------
    void HybridRenderingLayer::setup_lights()
    {
        int32 nr_lights = std::clamp(hybrid_rendering::LAYER_DESCRIPTION.nr_lights, hybrid_rendering::MIN_NR_LIGHTS, hybrid_rendering::MAX_NR_LIGHTS);

        srand(13); // seed random number generator
        for (int32 i = 0; i < nr_lights; ++i)
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
    void HybridRenderingLayer::setup_geometry()
    {
        const int32 MAX_OBJECTS = 1;

        std::array<rex::vec3, MAX_OBJECTS> object_positions = 
        {
            rex::vec3(0.0, -0.0, 0.0)
        };

        auto mesh_type = (geometry_rendering::MeshType)geometry_rendering::LAYER_DESCRIPTION.mesh_type;
        auto mesh_name = geometry_rendering::MESH_NAME_MAP[mesh_type];
        auto mesh_scale = geometry_rendering::MESH_SCALE_MAP[mesh_type];

        rex::ref_ptr<rex::Model> model = rex::model_library::get(mesh_name);
        if (model == nullptr)
        {
            return;
        }

        for (int32 i = 0; i < MAX_OBJECTS; ++i)
        {
            rex::ecs::Entity entity = m_scene->create_entity(mesh_name);

            entity.add_component<rex::ecs::ModelComponent>(model);
            entity.add_component<rex::ecs::MaterialComponent>(m_material);

            rex::ecs::TransformComponent& transform_comp = entity.get_component<rex::ecs::TransformComponent>();

            transform_comp.transform.set_position(object_positions[i]);
            transform_comp.transform.set_scale(mesh_scale);

            m_entities.push_back(entity);
        }
    }

    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> HybridRenderingLayer::create_deferred_geometry_pass(const rex::DeferredGeometryPassOptions& options) const
    {
        return std::make_unique<rex::DeferredGeometryPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> HybridRenderingLayer::create_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const
    {
        return std::make_unique<rex::DistanceEvaluationPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> HybridRenderingLayer::create_heatmap_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const
    {
        return std::make_unique<rex::HeatMapDistanceEvaluationPass>("color_ramp"_sid, options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> HybridRenderingLayer::create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const
    {
        return std::make_unique<rex::DeferredLightPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> HybridRenderingLayer::create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const
    {
        return std::make_unique<rex::DeferredLightVisualizationPass>(options, rex::CreateFrameBuffer::YES);
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<rex::SceneRenderPass> HybridRenderingLayer::create_composite_pass(const rex::CompositePassOptions& options) const
    {
        return std::make_unique<rex::CompositePass>(options, rex::CreateFrameBuffer::NO);
    }
} // namespace regina