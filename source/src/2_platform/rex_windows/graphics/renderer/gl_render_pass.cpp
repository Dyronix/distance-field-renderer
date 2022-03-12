#include "rex_windows_pch.h"

#include "graphics/renderer/gl_render_pass.h"

#include "renderer/renderer.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        RenderPass::RenderPass(const RenderPassDescription& description)
            :m_renderpass_description(description)
        {
            RENDERER_INFO("Creating Render Pass: {0}", description.name.to_string());
        }
        //-------------------------------------------------------------------------
        RenderPass::~RenderPass()
        {
            RENDERER_INFO("Destroying Render Pass: {0}", m_renderpass_description.name.to_string());

            m_renderpass_description.framebuffer.reset();
        }

        //-------------------------------------------------------------------------
        void RenderPass::resize(uint32 width, uint32 height)
        {
            if (m_renderpass_description.framebuffer)
            {
                m_renderpass_description.framebuffer->resize(width, height);
            }
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::FrameBuffer>& RenderPass::get_frame_buffer()
        {
            return m_renderpass_description.framebuffer;
        }
        //-------------------------------------------------------------------------
        const ref_ptr<rex::FrameBuffer>& RenderPass::get_frame_buffer() const
        {
            return m_renderpass_description.framebuffer;
        }
        //-------------------------------------------------------------------------
        const rex::ClearColor& RenderPass::get_clear_color() const
        {
            return m_renderpass_description.clear_color;
        }
        //-------------------------------------------------------------------------
        const rex::ClearDepth& RenderPass::get_clear_depth() const
        {
            return m_renderpass_description.clear_depth;
        }
        //-------------------------------------------------------------------------
        const rex::ClearFlags& RenderPass::get_clear_flags() const
        {
            return m_renderpass_description.clear_flags;
        }

        //-------------------------------------------------------------------------
        const rex::StringID& RenderPass::get_debug_name() const
        {
            return m_renderpass_description.name;
        }
    }
}