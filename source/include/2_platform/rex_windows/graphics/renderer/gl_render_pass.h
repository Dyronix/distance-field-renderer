#pragma once

#include "renderer/render_pass.h"

namespace rex
{
    namespace opengl
    {
        class RenderPass : public rex::RenderPass
        {
        public:
            RenderPass(const RenderPassDescription& description);
            ~RenderPass();

            void                                resize(uint32 width, uint32 height) override;

            ref_ptr<rex::FrameBuffer>&          get_frame_buffer() override;
            const ref_ptr<rex::FrameBuffer>&    get_frame_buffer() const override;

            const ClearColor&                   get_clear_color() const override;
            const ClearDepth&                   get_clear_depth() const override;
            const ClearFlags&                   get_clear_flags() const override;

            const StringID&                     get_debug_name() const override;

        private:
            RenderPassDescription               m_renderpass_description;
        };
    }
}