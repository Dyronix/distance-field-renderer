#include "rex_scenegraph_pch.h"

#include "renderpasses/distance_evaluation_pass.h"
#include "renderpasses/distance_evaluation_pass_helpers.h"

#include "scene_renderer.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/environment.h"

#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/uniform_buffer_set.h"

#include "ecs/scene_camera.h"
#include "ecs/scene.h"

#include "renderer/point_light.h"

namespace rex
{
    //-------------------------------------------------------------------------
    DistanceEvaluationPass::DistanceEvaluationPass(const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    DistanceEvaluationPass::~DistanceEvaluationPass()
    {
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if(m_create_framebuffer)
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription sdf_framebuffer_desc;

            sdf_framebuffer_desc.width = vp_width;
            sdf_framebuffer_desc.height = vp_height;
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.depth_attachment = std::move(create_depth_attachment_description(vp_width, vp_height, Texture::Format::DEPTH_COMPONENT_24_INTEGER));
            sdf_framebuffer_desc.name = "SDF";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(sdf_framebuffer_desc), FrameBufferDepthAttachmentOption::DEPTH_ONLY);
        }

        RenderPassDescription sdf_renderpass_desc;

        sdf_renderpass_desc.framebuffer = framebuffer;
        sdf_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        sdf_renderpass_desc.clear_depth = 1.0f;
        sdf_renderpass_desc.clear_flags = CLEAR_COLOR_AND_DEPTH;
        sdf_renderpass_desc.name = "SDF";

        PipelineDescription sdf_pipeline_desc;

        sdf_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        sdf_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        sdf_pipeline_desc.renderpass = Renderer::create_render_pass(sdf_renderpass_desc);
        sdf_pipeline_desc.depth_test_state = {DepthTestEnabled::YES};
        sdf_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        sdf_pipeline_desc.name = "SDF";

        create_pipeline(sdf_pipeline_desc);
        create_material(sdf_pipeline_desc.shader, "SDF");

        upload_sphere_tracer_options(m_options.sphere_tracer_options);
        upload_sdf_scene_options(m_options.sdf_scene_options);
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        rex::matrix4 frustrum = sdf::generate_frustrum_matrix(camera.get_aspect_ratio(), camera.get_perspective_field_of_view());

        auto material = get_material();

        material->set("u_camera_frustrum", frustrum);
        material->set("u_camera_position", cameraTransform.get_position());
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);
        Renderer::submit_fullscreen_quad(get_pipeline(), UniformBufferSet::instance(), get_material());
        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::end()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::set_sphere_tracer_options(const sdf::SphereTracerOptions& sphereTracerOptions)
    {
        m_options.sphere_tracer_options = sphereTracerOptions;
        upload_sphere_tracer_options(m_options.sphere_tracer_options);
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::set_sdf_scene_options(const sdf::SceneOptions& sdfSceneOptions)
    {
        m_options.sdf_scene_options = sdfSceneOptions;
        upload_sdf_scene_options(m_options.sdf_scene_options);
    }

    //-------------------------------------------------------------------------
    const rex::sdf::SphereTracerOptions& DistanceEvaluationPass::get_sphere_tracer_options() const
    {
        return m_options.sphere_tracer_options;
    }

    //-------------------------------------------------------------------------
    const rex::sdf::SceneOptions& DistanceEvaluationPass::get_sdf_scene_options() const
    {
        return m_options.sdf_scene_options;
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::upload_sphere_tracer_options(const sdf::SphereTracerOptions& sphereTracerOptions)
    {
        auto material = get_material();

        // Sphere Tracer
        R_ASSERT_X(sphereTracerOptions.max_iterations != 0, "No sphere tracer max iterations was given to the \"Distance Evaluation Pass\"");
        R_ASSERT_X(sphereTracerOptions.max_march_distance != 0.0f, "No sphere tracer max march distance was given to the \"Distance Evaluation Pass\"");
        R_ASSERT_X(sphereTracerOptions.min_surface_distance != 0.0f, "No sphere tracer min surface distance was given to the \"Distance Evaluation Pass\"");

        material->set("u_max_iterations", sphereTracerOptions.max_iterations);
        material->set("u_max_march_distance", sphereTracerOptions.max_march_distance);
        material->set("u_min_surface_distance", sphereTracerOptions.min_surface_distance);
    }
    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::upload_sdf_scene_options(const sdf::SceneOptions& sdfSceneOptions)
    {
        auto material = get_material();

        // Scene
        // Scene Scale can be equal to 1.0
        // Scene Offsetset can be equal to 0.0
        R_ASSERT_X(sdfSceneOptions.scene_size != vec3(0.0f, 0.0f, 0.0f), "No scene size was given to the \"Distance Evaluation Pass\"");
        // Scene Center can be located at vec3(0.0f, 0.0f, 0.0f).

        material->set("u_scene_scale", sdfSceneOptions.scene_scale);
        material->set("u_scene_offset", sdfSceneOptions.scene_offset);
        material->set("u_scene_size", sdfSceneOptions.scene_size);
        material->set("u_scene_center", sdfSceneOptions.scene_center);

        R_ASSERT_X(sdfSceneOptions.scene_voxel_grid_min_bounds != vec3(0.0f, 0.0f, 0.0f), "No scene minimum bounds was given to the \"Distance Evaluation Pass\"");
        R_ASSERT_X(sdfSceneOptions.scene_voxel_grid_max_bounds != vec3(0.0f, 0.0f, 0.0f), "No scene maximum bounds was given to the \"Distance Evaluation Pass\"");
        R_ASSERT_X(sdfSceneOptions.scene_voxel_grid_size != vec3(0.0f, 0.0f, 0.0f), "No scene voxel grid was given to the \"Distance Evaluation Pass\"");
        R_ASSERT_X(sdfSceneOptions.scene_voxel_grid_cell_size != vec3(0.0f, 0.0f, 0.0f), "No scene voxel grid cell size was given to the \"Distance Evaluation Pass\"");

        material->set("u_scene_voxel_grid_min_bounds", sdfSceneOptions.scene_voxel_grid_min_bounds);
        // material->set("u_scene_voxel_grid_max_bounds", sdfSceneOptions.scene_voxel_grid_max_bounds);
        material->set("u_scene_voxel_grid_size", sdfSceneOptions.scene_voxel_grid_size);
        material->set("u_scene_voxel_grid_cell_size", sdfSceneOptions.scene_voxel_grid_cell_size);

        R_ASSERT_X(sdfSceneOptions.scene_data != nullptr, "No scene data was given to the \"Distance Evaluation Pass\"");

        material->set_texture3d("u_scene_data", sdfSceneOptions.scene_data);
    }

    //-------------------------------------------------------------------------
    const rex::DistanceEvaluationsPassOptions& DistanceEvaluationPass::get_options()
    {
        return m_options;
    }

    //-------------------------------------------------------------------------
    bool DistanceEvaluationPass::should_create_frame_buffer() const
    {
        return m_create_framebuffer;
    }
}