#include "rex_scenegraph_pch.h"

#include "renderpasses/blur_pass.h"
#include "is_power_of_two.h"

#include "scene_renderer.h"

#include "perspective_camera.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"

#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/uniform_buffer_set.h"

#include "ecs/scene_camera.h"

namespace rex
{
    //-------------------------------------------------------------------------
    BlurPass::BlurPass(const BlurPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
        R_ASSERT(is_power_of_two(options.blur_width));
        R_ASSERT(is_power_of_two(options.blur_height));
    }

    //-------------------------------------------------------------------------
    BlurPass::~BlurPass()
    {
    }

    //-------------------------------------------------------------------------
    void BlurPass::on_initialize(const ref_ptr<rex::SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            FrameBufferDescription blur_framebuffer_desc;

            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            blur_framebuffer_desc.width = vp_width;
            blur_framebuffer_desc.height = vp_height;
            blur_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            blur_framebuffer_desc.name = "Blur";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(blur_framebuffer_desc), FrameBufferDepthAttachmentOption::NONE);
        }

        RenderPassDescription blur_renderpass_desc;

        blur_renderpass_desc.framebuffer = framebuffer;
        blur_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        blur_renderpass_desc.clear_flags = COLOR_ONLY;
        blur_renderpass_desc.clear_depth = 1.0f;
        blur_renderpass_desc.name = "Blur";

        PipelineDescription blur_pipeline_desc;

        blur_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        blur_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        blur_pipeline_desc.renderpass = Renderer::create_render_pass(blur_renderpass_desc);
        blur_pipeline_desc.name = "Blur";
        blur_pipeline_desc.depth_test_state = {DepthTestEnabled::NO};
        blur_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};

        create_pipeline(blur_pipeline_desc);
        create_material(blur_pipeline_desc.shader, "Simple Blur");
    }

    //-------------------------------------------------------------------------
    void BlurPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void BlurPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);

        get_material()->set("u_kernel_width", m_options.blur_width);
        get_material()->set("u_kernel_height", m_options.blur_width);
    }

    //-------------------------------------------------------------------------
    void BlurPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);

        setColor();

        Renderer::submit_fullscreen_quad(get_pipeline(), UniformBufferSet::instance(), get_material());
        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void BlurPass::end()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void BlurPass::setColor()
    {
        set_render_target("u_input", m_options.color_buffer, Renderer::get_black_texture());
    }
}