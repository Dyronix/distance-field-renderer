#include "rex_scenegraph_pch.h"

#include "renderpasses/composite_pass.h"

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
    CompositePass::CompositePass(const CompositePassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    CompositePass::~CompositePass()
    {
    }

    //-------------------------------------------------------------------------
    void CompositePass::set_gamma(float gamma)
    {
        get_material()->set("u_GammaCorrection", gamma);
    }

    //-------------------------------------------------------------------------
    void CompositePass::set_apply_gamma_correction(ApplyGammaCorrection apply)
    {
        get_material()->set("u_ApplyGammaCorrection", apply);
    }

    //-------------------------------------------------------------------------
    void CompositePass::set_apply_tone_mapping(ApplyToneMapping apply)
    {
        get_material()->set("u_ApplyToneMapping", apply);
    }

    //-------------------------------------------------------------------------
    void CompositePass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription composite_framebuffer_desc;

            composite_framebuffer_desc.width = vp_width;
            composite_framebuffer_desc.height = vp_height;
            composite_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            composite_framebuffer_desc.name = "Composite";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(composite_framebuffer_desc), FrameBufferDepthAttachmentOption::NONE);
        }

        RenderPassDescription composite_renderpass_desc;

        composite_renderpass_desc.framebuffer = framebuffer;
        composite_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        composite_renderpass_desc.clear_flags = COLOR_ONLY;
        composite_renderpass_desc.clear_depth = 1.0f;
        composite_renderpass_desc.name = "Composite";

        PipelineDescription composite_pipeline_desc;

        composite_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        composite_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        composite_pipeline_desc.renderpass = Renderer::create_render_pass(composite_renderpass_desc);
        composite_pipeline_desc.name = "Composite";
        composite_pipeline_desc.depth_test_state = {DepthTestEnabled::NO};
        composite_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};

        create_pipeline(composite_pipeline_desc);

        auto m = create_material(composite_pipeline_desc.shader, "Composite");

        m->set("u_ApplyToneMapping", m_options.apply_tone_mapping);
        m->set("u_ApplyGammaCorrection", m_options.apply_gamma_correction);

        if (m_options.apply_gamma_correction)
        {
            m->set("u_GammaCorrection", m_options.gamma_correction);
        }
    }

    //-------------------------------------------------------------------------
    void CompositePass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void CompositePass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
    }

    //-------------------------------------------------------------------------
    void CompositePass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);

        set_color();

        Renderer::submit_fullscreen_quad(get_pipeline(), UniformBufferSet::instance(), get_material());
        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void CompositePass::end()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void CompositePass::set_color()
    {
        set_render_target("u_Texture", m_options.color_buffer, Renderer::get_black_texture());
    }
}