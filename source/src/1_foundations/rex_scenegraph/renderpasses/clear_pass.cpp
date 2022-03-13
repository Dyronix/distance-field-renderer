#include "rex_scenegraph_pch.h"

#include "renderpasses/clear_pass.h"

#include "scene_renderer.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"

#include "ecs/scene_camera.h"

namespace rex
{
    //-------------------------------------------------------------------------
    ClearPass::ClearPass(const ClearPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    ClearPass::~ClearPass()
    {
    }

    //-------------------------------------------------------------------------
    void ClearPass::on_initialize(const ref_ptr<rex::SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            FrameBufferDescription clear_framebuffer_desc;

            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            clear_framebuffer_desc.width = vp_width;
            clear_framebuffer_desc.height = vp_height;
            clear_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            clear_framebuffer_desc.name = "Clear";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(clear_framebuffer_desc), FrameBufferDepthAttachmentOption::NONE);
        }

        RenderPassDescription clear_renderpass_desc;

        clear_renderpass_desc.framebuffer = framebuffer;
        clear_renderpass_desc.clear_color = {m_options.clear_color.red, m_options.clear_color.green, m_options.clear_color.blue, m_options.clear_color.alpha};
        clear_renderpass_desc.clear_flags = COLOR_ONLY;
        clear_renderpass_desc.clear_depth = 1.0f;
        clear_renderpass_desc.name = "Clear";

        PipelineDescription clear_pipeline_desc;

        clear_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        clear_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        clear_pipeline_desc.renderpass = Renderer::create_render_pass(clear_renderpass_desc);
        clear_pipeline_desc.name = "Clear";
        clear_pipeline_desc.depth_test_state = {DepthTestEnabled::NO};
        clear_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};

        create_pipeline(clear_pipeline_desc);
    }

    //-------------------------------------------------------------------------
    void ClearPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void ClearPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
    }

    //-------------------------------------------------------------------------
    void ClearPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);
        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void ClearPass::end()
    {
        // Nothing to implement
    }
}