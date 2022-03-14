#include "rex_scenegraph_pch.h"

#include "renderpasses/deferred_light_pass.h"

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

namespace rex
{
    //-------------------------------------------------------------------------
    DeferredLightPass::DeferredLightPass(const DeferredLightPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    DeferredLightPass::~DeferredLightPass()
    {
    }

    //-------------------------------------------------------------------------
    void DeferredLightPass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription deferredlight_framebuffer_desc;

            deferredlight_framebuffer_desc.width = vp_width;
            deferredlight_framebuffer_desc.height = vp_height;
            deferredlight_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            deferredlight_framebuffer_desc.depth_attachment = std::move(create_depth_attachment_description(vp_width, vp_height, Texture::Format::DEPTH_COMPONENT_32_FLOAT));
            deferredlight_framebuffer_desc.name = "DeferredLight";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(deferredlight_framebuffer_desc), FrameBufferDepthAttachmentOption::DEPTH_ONLY);
        }

        RenderPassDescription deferredlight_renderpass_desc;

        deferredlight_renderpass_desc.framebuffer = framebuffer;
        deferredlight_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        deferredlight_renderpass_desc.clear_flags = CLEAR_COLOR_AND_DEPTH;
        deferredlight_renderpass_desc.clear_depth = 1.0f;
        deferredlight_renderpass_desc.name = "DeferredLight";

        PipelineDescription deferredlight_pipeline_desc;

        deferredlight_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        deferredlight_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        deferredlight_pipeline_desc.renderpass = Renderer::create_render_pass(deferredlight_renderpass_desc);
        deferredlight_pipeline_desc.name = "DeferredLight";
        deferredlight_pipeline_desc.depth_test_state = {DepthTestEnabled::YES, DepthTestFunction::ALWAYS, DepthBufferReadOnly::YES};
        deferredlight_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};

        create_pipeline(deferredlight_pipeline_desc);
        create_material(deferredlight_pipeline_desc.shader, "DeferredLight");
    }

    //-------------------------------------------------------------------------
    void DeferredLightPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DeferredLightPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
    }

    //-------------------------------------------------------------------------
    void DeferredLightPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);

        set_g_position();
        set_g_normal();
        set_g_albedo_spec();

        Renderer::submit_fullscreen_quad(get_pipeline(), UniformBufferSet::instance(), get_material());
        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void DeferredLightPass::end()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DeferredLightPass::set_g_position()
    {
        set_render_target("u_Texture_G_Position", m_options.g_position_buffer, Renderer::get_black_texture());
    }
    //-------------------------------------------------------------------------
    void DeferredLightPass::set_g_normal()
    {
        set_render_target("u_Texture_G_Normal", m_options.g_normal_buffer, Renderer::get_black_texture());
    }
    //-------------------------------------------------------------------------
    void DeferredLightPass::set_g_albedo_spec()
    {
        set_render_target("u_Texture_G_AlbedoSpec", m_options.g_albedo_spec_buffer, Renderer::get_black_texture());
    }
}