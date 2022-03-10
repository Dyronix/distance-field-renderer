#pragma once

#include "allocators/allocator.h"

#include "memory_size.h"

namespace rex
{
    namespace memory
    {
        struct MemoryHeader;

        class LinearAllocator : public Allocator
        {
        public:
            explicit LinearAllocator(const MemoryHeader& block);
            ~LinearAllocator() override;

            void* allocate(const size& size) final;
            void deallocate(void* ptr) final;

            size get_used_size() const final;
            uint64 get_number_of_allocations() const final;

        protected:
            void on_clear() final;
            void on_free() final;


        private:
            void* m_curr_stack_ptr;

            size m_used_memory;
            uint64 m_allocations;
        };

        //-------------------------------------------------------------------------
        inline LinearAllocator create_linear_allocator(size size)
        {
            return create_allocator<LinearAllocator>(size);
        }
    }
}