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

            static ref_ptr<rex::FrameBuffer> create(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption = FrameBufferDepthAttachmentOption::NONE);

            FrameBuffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption = rex::FrameBufferDepthAttachmentOption::NONE);
            ~FrameBuffer() override;

            StringID get_name() const override;

            int32 get_width() const override;
            int32 get_height() const override;

            const std::vector<ref_ptr<Texture>> get_color_attachments() const override;
            const ref_ptr<Texture> get_color_attachment(int32 attachmentIndex = 0) const override;
            const ref_ptr<Texture> get_depth_attachment() const override;

            void resize(int32 width, int32 height, IsRenderThread isRenderThread = IsRenderThread::NO) override;

            void release() override;
            void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;
            void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const override;
            bool is_bound() const override;
            uint32 get_id() const override;

        private:
            void invalidate(ColorAttachmentDescriptions&& colorDescriptions, DepthAttachmentDescription&& depthDescription, rex::FrameBufferDepthAttachmentOption depthAttachmentOption = FrameBufferDepthAttachmentOption::NONE,
                            IsRenderThread isRenderThread = IsRenderThread::NO);

            StringID m_name;

            int32 m_width;
            int32 m_height;
            uint32 m_buffer_id;

            mutable bool m_is_bound;

            std::vector<ref_ptr<RenderTarget>> m_color_attachments;
            ref_ptr<RenderTarget> m_depth_attachment;

            rex::FrameBufferDepthAttachmentOption m_depth_attachment_option;
        };
    }
}