#include "rex_memory_pch.h"
#include "allocators/allocator.h"
#include "pointer_math.h"

namespace rex
{
    namespace memory
    {
        //-------------------------------------------------------------------------
        Allocator::Allocator(const MemoryHeader& block)
            :m_memory_block(block)
        {
            R_ASSERT_X(get_memory().alignment != 0u, "No alignment specified");
        }
        //-------------------------------------------------------------------------
        Allocator::~Allocator()
        {
            m_memory_block.blob.release();
        }

        //-------------------------------------------------------------------------
        void Allocator::clear()
        {
            on_clear();
        }

        //-------------------------------------------------------------------------
        void Allocator::free()
        {
            m_memory_block.blob.release();

            on_free();
        }

        //-------------------------------------------------------------------------
        void* Allocator::get_base_pointer()
        {
            return m_memory_block.blob.get_data_as<void*>();
        }

        //-------------------------------------------------------------------------
        const void* Allocator::get_base_pointer() const
        {
            return m_memory_block.blob.get_data_as<const void*>();
        }

        //-------------------------------------------------------------------------
        void* Allocator::get_aligned_base_pointer()
        {
            void* base = get_base_pointer();

            uint64 alignment_offset = pointer_math::alignment_offset(base, get_alignment());

            return pointer_math::jump_forward(base, alignment_offset);
        }

        //-------------------------------------------------------------------------
        const void* Allocator::get_aligned_base_pointer() const
        {
            const void* base = get_base_pointer();

            uint64 alignment_offset = pointer_math::alignment_offset(base, get_alignment());

            return pointer_math::jump_forward(base, alignment_offset);
        }

        //-------------------------------------------------------------------------
        size Allocator::get_total_size() const
        {
            return m_memory_block.blob.get_size();
        }

        //-------------------------------------------------------------------------
        uint64 Allocator::get_alignment() const
        {
            return m_memory_block.alignment;
        }

        //-------------------------------------------------------------------------
        bool Allocator::can_allocate(const size& size) const
        {
            return get_used_size() + size < get_total_size();
        }

        //-------------------------------------------------------------------------
        const MemoryHeader& Allocator::get_memory() const
        {
            return m_memory_block;
        }
    }
}