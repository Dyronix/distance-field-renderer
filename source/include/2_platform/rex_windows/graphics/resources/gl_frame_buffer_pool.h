#pragma once

#include "resources/frame_buffer_pool.h"

namespace rex
{
    namespace opengl
    {
        class FrameBufferPool : public rex::FrameBufferPool
        {
        public:
            FrameBufferPool();
            ~FrameBufferPool() override;

        protected:
            ref_ptr<rex::FrameBuffer> create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption) const override;
        };
    }
}