#pragma once

#include "resources/frame_buffer_pool.h"
#include "resources/frame_buffer.h"

namespace rex
{
    class FrameBufferPool : public RefCountedObject
    {
    public:
        using PoolIt = std::vector<ref_ptr<rex::FrameBuffer>>::iterator;
        using ConstPoolIt = std::vector<ref_ptr<rex::FrameBuffer>>::const_iterator;
        using ReversePoolIt = std::vector<ref_ptr<rex::FrameBuffer>>::reverse_iterator;
        using ConstReversePoolIt = std::vector<ref_ptr<rex::FrameBuffer>>::const_reverse_iterator;

        static FrameBufferPool* instance();

        FrameBufferPool(uint32 max = 32u);
        virtual ~FrameBufferPool();

        //
        inline PoolIt                 begin() { return m_framebuffer_pool.begin(); }
        inline ConstPoolIt            begin() const { return m_framebuffer_pool.begin(); }
        inline ConstPoolIt            cbegin() const { return begin(); }
        inline PoolIt                 end() { return m_framebuffer_pool.end(); }
        inline ConstPoolIt            end() const { return m_framebuffer_pool.end(); }
        inline ConstPoolIt            cend() const { return end(); }

        inline ReversePoolIt          rbegin() { return m_framebuffer_pool.rbegin(); }
        inline ConstReversePoolIt     rbegin() const { return m_framebuffer_pool.rbegin(); }
        inline ConstReversePoolIt     crbegin() const { return rbegin(); }
        inline ReversePoolIt          rend() { return m_framebuffer_pool.rend(); }
        inline ConstReversePoolIt     rend() const { return rend(); }
        inline ConstReversePoolIt     crend() const { return m_framebuffer_pool.crend(); }

        void clear();
        void add(const ref_ptr<FrameBuffer>& buffer);

        uint32 size() const;
        uint32 capacity() const;

        ref_ptr<FrameBuffer> get(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption);

    protected:
        virtual ref_ptr<FrameBuffer> create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption) const = 0;

    private:
        std::vector<ref_ptr<rex::FrameBuffer>> m_framebuffer_pool;
        static FrameBufferPool* s_instance;
    };
}