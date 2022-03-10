#pragma once

#include "resources/frame_buffer_pool.h"
#include "resources/frame_buffer.h"

namespace sbt
{
    class FrameBufferPool : public RefCountedObject
    {
    public:
        using PoolIt = std::vector<ref_ptr<sbt::FrameBuffer>>::iterator;
        using ConstPoolIt = std::vector<ref_ptr<sbt::FrameBuffer>>::const_iterator;
        using ReversePoolIt = std::vector<ref_ptr<sbt::FrameBuffer>>::reverse_iterator;
        using ConstReversePoolIt = std::vector<ref_ptr<sbt::FrameBuffer>>::const_reverse_iterator;

        static FrameBufferPool* instance();

        FrameBufferPool(uint32 max = 32u);
        virtual ~FrameBufferPool();

        PoolIt begin()
        {
            return m_framebuffer_pool.begin();
        }
        ConstPoolIt cbegin() const
        {
            return m_framebuffer_pool.cbegin();
        }
        PoolIt end()
        {
            return m_framebuffer_pool.end();
        }
        ConstPoolIt cend() const
        {
            return m_framebuffer_pool.cend();
        }

        ReversePoolIt rbegin()
        {
            return m_framebuffer_pool.rbegin();
        }
        ConstReversePoolIt crbegin() const
        {
            return m_framebuffer_pool.crbegin();
        }
        ReversePoolIt rend()
        {
            return m_framebuffer_pool.rend();
        }
        ConstReversePoolIt crend() const
        {
            return m_framebuffer_pool.crend();
        }

        void clear();
        void add(const ref_ptr<FrameBuffer>& buffer);

        uint32 size() const;
        uint32 capacity() const;

        ref_ptr<FrameBuffer> get(FrameBufferDescription&& description, DepthAttachmentOption depthAttachmentOption);

    protected:
        virtual ref_ptr<FrameBuffer> createFrameBuffer(FrameBufferDescription&& description, DepthAttachmentOption depthAttachmentOption) const = 0;

    private:
        std::vector<ref_ptr<sbt::FrameBuffer>> m_framebuffer_pool;
        static FrameBufferPool* s_instance;
    };
}