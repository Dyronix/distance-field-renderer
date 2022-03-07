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

    class FrameBuffer : public Resource
    {
    public:
        using ColorAttachment = NamedType<ref_ptr<Texture>, struct CAttachment>;
        using DepthAttachment = NamedType<ref_ptr<Texture>, struct DAttachment>;

        using ColorAttachments = std::vector<ColorAttachment>;

        enum class DepthAttachmentOption
        {
            NONE,
            DEPTH_ONLY,
            STENCIL_ONLY,
            DEPTH_STENCIL
        };

        virtual ~FrameBuffer() = default;

        virtual StringID getName() const = 0;

        virtual int32 getWidth() const = 0;
        virtual int32 getHeight() const = 0;

        virtual const ColorAttachments getColorAttachments() const = 0;
        virtual const ColorAttachment getColorAttachment(int32 attachmentIndex = 0) const = 0;
        virtual const DepthAttachment getDepthAttachment() const = 0;

        virtual void resize(int32 width, int32 height, bool isRenderThread = IsRenderThread::NO) = 0;

        virtual void bind(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;
        virtual void unbind(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;
    };
}