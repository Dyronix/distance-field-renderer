#include "rex_scenegraph_pch.h"

#include "renderpasses/heatmap_distance_evaluation_pass.h"
#include "renderpasses/distance_evaluation_pass_helpers.h"

#include "scene_renderer.h"

#include "renderer/environment.h"
#include "renderer/pipeline.h"
#include "renderer/render_pass.h"
#include "renderer/renderer.h"

#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/uniform_buffer_set.h"

#include "ecs/scene.h"
#include "ecs/scene_camera.h"

#include "renderer/point_light.h"

namespace rex
{
    //-------------------------------------------------------------------------
    HeatMapDistanceEvaluationPass::HeatMapDistanceEvaluationPass(const StringID& colorRampName, const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : DistanceEvaluationPass(options, create_frame_buffer)
        , m_color_ramp_name(colorRampName)
    {
    }

    //-------------------------------------------------------------------------
    HeatMapDistanceEvaluationPass::~HeatMapDistanceEvaluationPass()
    {
    }

    //-------------------------------------------------------------------------
    void HeatMapDistanceEvaluationPass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (should_create_frame_buffer())
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription sdf_framebuffer_desc;

            sdf_framebuffer_desc.width = vp_width;
            sdf_framebuffer_desc.height = vp_height;
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.name = "HeatMap SDF";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(sdf_framebuffer_desc), FrameBufferDepthAttachmentOption::NONE);
        }

        RenderPassDescription sdf_renderpass_desc;

        sdf_renderpass_desc.framebuffer = framebuffer;
        sdf_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        sdf_renderpass_desc.clear_depth = 1.0f;
        sdf_renderpass_desc.clear_flags = COLOR_ONLY;
        sdf_renderpass_desc.name = "HeatMap SDF";

        PipelineDescription sdf_pipeline_desc;

        sdf_pipeline_desc.shader = shader_library::get(get_options().shader_name);
        sdf_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        sdf_pipeline_desc.renderpass = Renderer::create_render_pass(sdf_renderpass_desc);
        sdf_pipeline_desc.depth_test_state = {DepthTestEnabled::NO};
        sdf_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        sdf_pipeline_desc.name = "HeatMap SDF";

        create_pipeline(sdf_pipeline_desc);
        create_material(sdf_pipeline_desc.shader, "HeatMap SDF");

        upload_sphere_tracer_options(get_options().sphere_tracer_options);
        upload_sdf_scene_options(get_options().sdf_scene_options);

        upload_color_ramp_texture(m_color_ramp_name);
    }

    //-------------------------------------------------------------------------
    void HeatMapDistanceEvaluationPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void HeatMapDistanceEvaluationPass::upload_color_ramp_texture(const StringID& textureName)
    {
        auto material = get_material();

        auto texture = texture_library::get(textureName);

        if (texture == nullptr)
        {
            R_ERROR("Color ramp texture was not found with name: {0}", textureName.to_string());
            texture = Renderer::get_white_texture();
        }

        material->set_texture2d("u_color_ramp", texture);
    }

} // namespace rex