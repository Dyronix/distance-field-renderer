#include "rex_windows_pch.h"

#include "graphics/resources/gl_frame_buffer_pool.h"
#include "graphics/resources/gl_frame_buffer.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        FrameBufferPool::FrameBufferPool() = default;
        //-------------------------------------------------------------------------
        FrameBufferPool::~FrameBufferPool() = default;

        //-------------------------------------------------------------------------
        ref_ptr<rex::FrameBuffer> FrameBufferPool::create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption) const
        {
            return ref_ptr<rex::FrameBuffer>(make_ref<opengl::FrameBuffer>(std::move(description), depthAttachmentOption));
        }
    }
}