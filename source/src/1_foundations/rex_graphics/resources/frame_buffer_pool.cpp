#include "rex_graphics_pch.h"

#include "resources/frame_buffer_pool.h"
#include "resources/resource_factory.h"

namespace rex
{
    //-------------------------------------------------------------------------
    rex::FrameBufferPool* FrameBufferPool::s_instance = nullptr;

    //-------------------------------------------------------------------------
    rex::FrameBufferPool* FrameBufferPool::instance()
    {
        return s_instance;
    }

    //-------------------------------------------------------------------------
    FrameBufferPool::FrameBufferPool(uint32 max)
    {
        if (s_instance != nullptr)
        {
            R_ERROR("Second instance of framebuffer pool created!");
            return;
        }

        s_instance = this;

        m_framebuffer_pool.reserve(max);
    }
    //-------------------------------------------------------------------------
    FrameBufferPool::~FrameBufferPool()
    {
        clear();
    }

    //-------------------------------------------------------------------------
    void FrameBufferPool::clear()
    {
        for (auto& framebuffer : m_framebuffer_pool)
        {
            framebuffer->release();
        }

        m_framebuffer_pool.clear();
    }

    //-------------------------------------------------------------------------
    void FrameBufferPool::add(const ref_ptr<FrameBuffer>& buffer)
    {
        if (m_framebuffer_pool.size() == capacity())
        {
            R_ERROR("Max amount of framebuffers reached");
            return;
        }

        m_framebuffer_pool.push_back(buffer);
    }

    //-------------------------------------------------------------------------
    uint32 FrameBufferPool::size() const
    {
        return gsl::narrow<uint32>(m_framebuffer_pool.size());
    }
    //-------------------------------------------------------------------------
    uint32 FrameBufferPool::capacity() const
    {
        return gsl::narrow<uint32>(m_framebuffer_pool.capacity());
    }

    //-------------------------------------------------------------------------
    ref_ptr<FrameBuffer> FrameBufferPool::get(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption)
    {
        if (m_framebuffer_pool.size() == capacity())
        {
            R_ERROR("Max amount of framebuffers reached");
            return nullptr;
        }

        auto shared_framebuffer = create_frame_buffer(std::move(description), depthAttachmentOption);

        m_framebuffer_pool.push_back(shared_framebuffer);

        return shared_framebuffer;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::FrameBuffer> FrameBufferPool::get_bound() const
    {
        R_ASSERT_X(std::count_if(std::cbegin(m_framebuffer_pool), std::cend(m_framebuffer_pool), [](const auto& framebuffer) { return framebuffer->is_bound(); }) == 1, "Only one framebuffer can be active, did you forget to call ::endRenderPass somewhere?");

        for (const ref_ptr<rex::FrameBuffer>& buffer : m_framebuffer_pool)
        {
            if (buffer->is_bound())
            {
                return buffer;
            }
        }

        return nullptr;
    }

}