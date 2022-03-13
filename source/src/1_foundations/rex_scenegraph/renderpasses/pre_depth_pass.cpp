#include "rex_scenegraph_pch.h"

#include "renderpasses/pre_depth_pass.h"

#include "scene_renderer.h"
#include "model.h"
#include "perspective_camera.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/uniform_buffer_set.h"
#include "resources/material.h"

#include "ecs/scene.h"
#include "ecs/scene_camera.h"
#include "ecs/components/model_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/material_component.h"

namespace rex
{
    //-------------------------------------------------------------------------
    PreDepthPass::PreDepthPass(const PreDepthPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    PreDepthPass::~PreDepthPass()
    {
    }

    //-------------------------------------------------------------------------
    void PreDepthPass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription predepth_framebuffer_desc;

            predepth_framebuffer_desc.width = vp_width;
            predepth_framebuffer_desc.height = vp_height;
            predepth_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            predepth_framebuffer_desc.depth_attachment = std::move(create_depth_attachment_description(vp_width, vp_height, Texture::Format::DEPTH_COMPONENT_32_FLOAT));
            predepth_framebuffer_desc.name = "PreDepth";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(predepth_framebuffer_desc), FrameBufferDepthAttachmentOption::DEPTH_ONLY);
        }

        RenderPassDescription predepth_renderpass_desc;

        predepth_renderpass_desc.framebuffer = framebuffer;
        predepth_renderpass_desc.clear_color = {m_options.far_plane, m_options.far_plane, m_options.far_plane, 1.0f};
        predepth_renderpass_desc.clear_depth = 1.0f;
        predepth_renderpass_desc.clear_flags = CLEAR_COLOR_AND_DEPTH;
        predepth_renderpass_desc.name = "PreDepth";

        PipelineDescription predepth_pipeline_desc;

        predepth_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        predepth_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC3, "a_Normal"}, {DataType::VEC3, "a_Tangent"}, {DataType::VEC3, "a_Binormal"}, {DataType::VEC2, "a_TexCoord"}};
        predepth_pipeline_desc.renderpass = Renderer::create_render_pass(predepth_renderpass_desc);
        predepth_pipeline_desc.depth_test_state = {DepthTestEnabled::YES, DepthTestFunction::LESS, DepthBufferReadOnly::NO};
        predepth_pipeline_desc.name = "PreDepth";

        if (m_options.backface_culling)
        {
            predepth_pipeline_desc.facecull_state = {FaceCullingEnabled::YES, FaceCullingCullMode::BACK, FaceWinding::COUNTER_CLOCKWISE};
        }
        else
        {
            predepth_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        }

        create_pipeline(predepth_pipeline_desc);
        create_material(predepth_pipeline_desc.shader, "PreDepth");
    }

    //-------------------------------------------------------------------------
    void PreDepthPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void PreDepthPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);

        get_material()->set("u_near_plane", m_options.near_plane);
        get_material()->set("u_far_plane", m_options.far_plane);
    }

    //-------------------------------------------------------------------------
    void PreDepthPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);

        for (auto& dc : get_scene_renderer()->get_draw_commands())
        {
            Renderer::render_model_with_material(get_pipeline(), UniformBufferSet::instance(), dc.model, dc.transform, get_material());
        }

        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void PreDepthPass::end()
    {
        // Nothing to implement
    }
}