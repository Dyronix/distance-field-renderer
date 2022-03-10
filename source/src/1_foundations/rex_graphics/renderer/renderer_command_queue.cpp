#include "rex_graphics_pch.h"

#include "renderer/renderer_command_queue.h"

#include "raii/scope_guard.h"

#include "pointer_math.h"

namespace rex
{
    //-------------------------------------------------------------------------
    RendererCommandQueue::RendererCommandQueue(const memory::size& size)
        : m_linear_allocator(memory::createAllocator<memory::LinearAllocator>(size))
        , m_queue_state(RenderCommandQueueState::ACCEPT)
        , m_command_allocations(0)
    {
    }

    //-------------------------------------------------------------------------
    RendererCommandQueue::~RendererCommandQueue()
    {
        m_linear_allocator.free();
    }

    //-------------------------------------------------------------------------
    rex::RenderCommandQueueState RendererCommandQueue::get_state() const
    {
        return m_queue_state;
    }

    //-------------------------------------------------------------------------
    void RendererCommandQueue::execute()
    {
        R_ASSERT(m_queue_state == RenderCommandQueueState::ACCEPT);

        // Reset values when exiting this function
        auto guard = make_scope_guard([&]() mutable
                                      {
                                          m_queue_state = RenderCommandQueueState::ACCEPT;
                                          m_command_allocations = 0;
                                      });

        m_queue_state = RenderCommandQueueState::EXECUTE;

        auto aligned_buffer = m_linear_allocator.get_aligned_base_pointer();

        for (int32 i = 0; i < m_command_allocations; ++i)
        {
            uint64 alignment_offset = 0;

            alignment_offset = pointer_math::alignment_offset(aligned_buffer, m_linear_allocator.get_alignment());
            aligned_buffer = pointer_math::jump_forward(aligned_buffer, alignment_offset);

            RenderCommand render_command = *(RenderCommand*)aligned_buffer;

            aligned_buffer = pointer_math::jump_forward(aligned_buffer, sizeof(RenderCommand));

            alignment_offset = pointer_math::alignment_offset(aligned_buffer, m_linear_allocator.get_alignment());
            aligned_buffer = pointer_math::jump_forward(aligned_buffer, alignment_offset);

            size_t sizeof_func_ptr = *(size_t*)aligned_buffer;

            aligned_buffer = pointer_math::jumpForward(aligned_buffer, sizeof_func_ptr);

            alignment_offset = pointer_math::alignment_offset(aligned_buffer, m_linear_allocator.get_alignment());
            aligned_buffer = pointer_math::jump_forward(aligned_buffer, alignment_offset);

            render_command(aligned_buffer);

            aligned_buffer = pointer_math::jump_forward(aligned_buffer, sizeof_func_ptr);
        }
    }
    //-------------------------------------------------------------------------
    void RendererCommandQueue::clear()
    {
        R_ASSERT(m_queue_state == RenderCommandQueueState::ACCEPT);

        auto guard = make_scope_guard([&]() mutable
                                      {
                                          m_queue_state = RenderCommandQueueState::ACCEPT;
                                      });

        m_queue_state = RenderCommandQueueState::CLEAR;

        m_linear_allocator.clear();
    }
}