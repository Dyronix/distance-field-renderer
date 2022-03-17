#pragma once

#include "resources/api_resource.h"

#include "resources/texture_2d_description.h"

namespace rex
{
    class Texture;

    using ColorAttachmentDescriptions = std::vector<Texture2DDescription>;
    using DepthAttachmentDescription = Texture2DDescription;

    struct FrameBufferDescription
    {
        int32 width;
        int32 height;

        ColorAttachmentDescriptions color_attachments;
        DepthAttachmentDescription depth_attachment;

        StringID name;
    };

    enum class FrameBufferDepthAttachmentOption
    {
        NONE,
        DEPTH_ONLY,
        STENCIL_ONLY,
        DEPTH_STENCIL
    };

    enum class FrameBufferCopyOption
    {
        COLOR,
        DEPTH,
        STENCIL
    };

    enum class FrameBufferFilterOption
    {
        NEAREST,
        LINEAR
    };

    class FrameBuffer : public Resource
    {
    public:
        virtual ~FrameBuffer() = default;

        virtual StringID get_name() const = 0;

        virtual int32 get_width() const = 0;
        virtual int32 get_height() const = 0;

        virtual const std::vector<ref_ptr<Texture>> get_color_attachments() const = 0;
        virtual const ref_ptr<Texture> get_color_attachment(int32 attachmentIndex = 0) const = 0;
        virtual const ref_ptr<Texture> get_depth_attachment() const = 0;

        virtual void resize(int32 width, int32 height, IsRenderThread isRenderThread = IsRenderThread::NO) = 0;

        virtual void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;
        virtual void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;
        virtual bool is_bound() const = 0;
        virtual uint32 get_id() const = 0;
    };
}