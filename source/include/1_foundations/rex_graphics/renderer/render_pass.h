#pragma once

#include "color.h"

#include "types/clear_flags.h"

namespace rex
{
    class FrameBuffer;

    using ClearColor = Color4;
    using ClearDepth = float;

    struct RenderPassDescription
    {
        ref_ptr<FrameBuffer> framebuffer;

        ClearColor clear_color;
        ClearDepth clear_depth;
        ClearFlags clear_flags;

        StringID name;
    };

    class RenderPass : public RefCountedObject
    {
    public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        virtual void resize(uint32 width, uint32 height) = 0;

        virtual ref_ptr<FrameBuffer>& get_frame_buffer() = 0;
        virtual const ref_ptr<FrameBuffer>& get_frame_buffer() const = 0;

        virtual const ClearColor& get_clear_color() const = 0;
        virtual const ClearDepth& get_clear_depth() const = 0;
        virtual const ClearFlags& get_clear_flags() const = 0;

        virtual const StringID& get_debug_name() const = 0;
    };
}