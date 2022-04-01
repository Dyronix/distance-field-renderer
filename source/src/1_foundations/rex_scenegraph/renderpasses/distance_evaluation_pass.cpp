#include "rex_scenegraph_pch.h"

#include "renderpasses/distance_evaluation_pass.h"

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
    DistanceEvaluationPass::DistanceEvaluationPass(const DistanceEvaluationPassOptions& options, CreateFrameBuffer create_frame_buffer)
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
            sdf_framebuffer_desc.name = "SDF";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(geometry_framebuffer_desc), FrameBufferDepthAttachmentOption::NONE);
        }

        RenderPassDescription sdf_renderpass_desc;

        sdf_renderpass_desc.framebuffer = framebuffer;
        sdf_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        sdf_renderpass_desc.clear_depth = 1.0f;
        sdf_renderpass_desc.clear_flags = CLEAR_ONLY;
        sdf_renderpass_desc.name = "SDF";

        PipelineDescription sdf_desc;

        sdf_desc.shader = shader_library::get(m_options.geometry_shader_name);
        sdf_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        sdf_desc.renderpass = Renderer::create_render_pass(geometry_renderpass_desc);
        sdf_desc.depth_test_state = {DepthTestEnabled::NO};
        sdf_desc.facecull_state = {FaceCullingEnabled::NO};
        sdf_desc.name = "SDF";

        create_pipeline(geometry_pipeline_desc);
        create_material(deferredlight_pipeline_desc.shader, "SDF");
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
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
}