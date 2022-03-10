#include "rex_memory_pch.h"
#include "allocators/linear_allocator.h"
#include "pointer_math.h"

namespace rex
{
    namespace memory
    {
        //-------------------------------------------------------------------------
        LinearAllocator::LinearAllocator(const MemoryHeader& block)
            : Allocator(block)
            , m_curr_stack_ptr(get_base_pointer())
            , m_used_memory(0_bytes)
            , m_allocations(0u)
        {}
        //-------------------------------------------------------------------------
        LinearAllocator::~LinearAllocator()
        {
            //
            // Memory leak detection
            //
            assert(get_used_size() == 0_bytes && "Memory leak detected!");
            assert(get_number_of_allocations() == 0u && "Memory leak detected!");
        }

        //-------------------------------------------------------------------------
        void* LinearAllocator::allocate(const memory::size& size)
        {
            assert(size != 0u && "No size specified");

            uint64 alignment_offset = pointer_math::alignment_offset(m_curr_stack_ptr, get_alignment());
            if (!can_allocate(size + alignment_offset))
            {
                assert(false && "Out of memory!");
                return nullptr;
            }

            //
            // Align the stack pointer
            //
            void* aligned_address = pointer_math::jump_forward(m_curr_stack_ptr, alignment_offset);

            //
            // Move the stack pointer with the allocated size
            //
            m_curr_stack_ptr = pointer_math::jump_forward(aligned_address, size.bytes());

            //
            // Update book keeping
            //
            m_used_memory = memory::size(pointer_math::diff(m_curr_stack_ptr, get_base_pointer()));
            m_allocations = m_allocations + 1;

            return aligned_address;
        }
        //-------------------------------------------------------------------------
        void LinearAllocator::deallocate(void*)
        {
            R_ASSERT_X(false, "Individual deallocations cannot be made in a linear allocators");
        }

        //-------------------------------------------------------------------------
        size LinearAllocator::get_used_size() const
        {
            return m_used_memory;
        }
        //-------------------------------------------------------------------------
        uint64 LinearAllocator::get_number_of_allocations() const
        {
            return m_allocations;
        }

        //-------------------------------------------------------------------------
        void LinearAllocator::on_clear()
        {
            m_curr_stack_ptr = get_base_pointer();

            m_used_memory = 0_bytes;
            m_allocations = 0u;
        }

        //-------------------------------------------------------------------------
        void LinearAllocator::on_free()
        {
            m_used_memory = 0_bytes;
            m_allocations = 0u;
        }
    }
}