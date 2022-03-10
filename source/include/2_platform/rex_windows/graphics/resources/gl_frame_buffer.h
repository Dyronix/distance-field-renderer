#pragma once

#include "resources/frame_buffer.h"

namespace rex
{
    namespace opengl
    {
        class RenderTarget;

        class FrameBuffer : public rex::FrameBuffer
        {
        public:
            using ColorAttachment = NamedType<ref_ptr<RenderTarget>, struct CAttachment>;
            using DepthAttachment = NamedType<ref_ptr<RenderTarget>, struct DAttachment>;

            using ColorAttachments = std::vector<ColorAttachment>;

            static ref_ptr<rex::FrameBuffer> create(FrameBufferDescription&& description,
                                                    DepthAttachmentOption depthAttachmentOption = DepthAttachmentOption::NONE);

            FrameBuffer(FrameBufferDescription&& description,
                        DepthAttachmentOption depthAttachmentOption = rex::FrameBuffer::DepthAttachmentOption::NONE);
            ~FrameBuffer() override;

            StringID get_name() const override;

            int32 get_width() const override;
            int32 get_height() const override;

            const rex::FrameBuffer::ColorAttachments get_color_attachments() const override;
            const rex::FrameBuffer::ColorAttachment get_color_attachment(uint32 attachmentIndex = 0) const override;
            const rex::FrameBuffer::DepthAttachment get_depth_attachment() const override;

            void resize(int32 width, int32 height, IsRenderThread isRenderThread = IsRenderThread::NO) override;

            void release() override;
            void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;
            void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;

        private:
            void invalidate(ColorAttachmentDescriptions&& colorDescriptions, DepthAttachmentDescription&& depthDescription,
                            rex::FrameBuffer::DepthAttachmentOption depthAttachmentOption = DepthAttachmentOption::NONE,
                            IsRenderThread isRenderThread = IsRenderThread::NO);

            StringID m_name;

            uint32 m_width;
            uint32 m_height;
            uint32 m_buffer_id;

            ColorAttachments m_color_attachments;
            DepthAttachment m_depth_attachment;

            rex::FrameBuffer::DepthAttachmentOption m_depth_attachment_option;
        };
    }
}