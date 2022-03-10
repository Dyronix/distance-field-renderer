#pragma once

#include "memory_size.h"

#include "allocators/linear_allocator.h"

namespace rex
{
    typedef void (*RenderCommand)(void*);

    enum class RenderCommandQueueState
    {
        ACCEPT,
        EXECUTE,
        CLEAR
    };

    class RendererCommandQueue
    {
    public:
        RendererCommandQueue(const memory::size& size = 1_mb);
        ~RendererCommandQueue();

        RenderCommandQueueState get_state() const;

        template <typename TFunction>
        void push(TFunction&& cmd);

        void execute();
        void clear();

    private:
        memory::LinearAllocator m_linear_allocator;
        RenderCommandQueueState m_queue_state;
        size_t m_command_allocations;
    };

    //-------------------------------------------------------------------------
    template <typename TFunction>
    void rex::RendererCommandQueue::push(TFunction&& cmd)
    {
        RenderCommand render_command = [](void* ptr)
        {
            auto function_ptr = (TFunction*)ptr;

            (*function_ptr)();

            function_ptr->~TFunction();
        };

        size_t sizeof_render_command = sizeof(render_command);
        void* storage_01 = m_linear_allocator.allocate(sizeof_render_command);
        *(RenderCommand*)storage_01 = render_command;

        size_t sizeof_func_ptr = sizeof(cmd);
        void* storage_02 = m_linear_allocator.allocate(sizeof_func_ptr);
        *(size_t*)storage_02 = sizeof_func_ptr;

        void* storage_03 = m_linear_allocator.allocate(sizeof_func_ptr);
        new (storage_03) TFunction(std::forward<TFunction>((TFunction && )cmd));

        ++m_command_allocations;
    }
}