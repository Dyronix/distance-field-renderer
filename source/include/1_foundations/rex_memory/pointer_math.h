#pragma once

#include "types.h"

#include <cstddef>

namespace rex
{
    namespace pointer_math
    {
        template<typename T>
        struct Header
        {
        public:
            //-------------------------------------------------------------------------
            Header()
                :m_size(sizeof(T))
            {}

            //-------------------------------------------------------------------------
            uint64 get_size() const { return m_size; }

        private:
            uint64 m_size;
        };

        //-------------------------------------------------------------------------
        /// <summary>
        /// Jump forward in memory, giving the input address a certain offset
        /// </summary>
        /// <param name="address">The address we will start from</param>
        /// <param name="alignment">The offset we will apply</param>
        /// <returns>The new address</returns>
        template <typename T>
        inline void* jump_forward(const void* address, T offset)
        {
            return (void*)((memory::byte*)address + offset);
        }

        //-------------------------------------------------------------------------
        /// <summary>
        /// Jump backward in memory, giving the input address a certain offset
        /// </summary>
        /// <param name="address">The address we will start from</param>
        /// <param name="alignment">The offset we will apply</param>
        /// <returns>The new address</returns>
        template <typename T>
        inline void* jump_backward(const void* address, T offset)
        {
            return (void*)((memory::byte*)address - offset);
        }

        //-------------------------------------------------------------------------
        /// <summary>
        /// Calculate the offset between to addresses
        /// </summary>
        /// <param name="address">The address we will start from</param>
        /// <param name="alignment">The address we will end at</param>
        /// <returns>The resulting offset</returns>
        inline uint64 diff(const void* from, const void* to)
        {
            return (memory::byte*)from - (memory::byte*)to;
        }

        //-------------------------------------------------------------------------
        /// <summary>
        /// Retrieve the offset our address needs to be aligned with the given alignment
        /// </summary>
        /// <param name="address">The adress we will align</param>
        /// <param name="alignment">The required alignment of the address</param>
        /// <returns>The offset required to align the address</returns>
        inline uint64 alignment_offset(const void* address, uint64 alignment)
        {
            uint64 adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));

            //
            // We are already aligned
            //
            if (adjustment == alignment)
                return 0; 

            return adjustment;
        }

        //-------------------------------------------------------------------------
        /// <summary>
        /// Retrieve the offset our address needs to be aligned with the given alignment
        /// </summary>
        /// <param name="address">The adress we will align</param>
        /// <param name="alignment">The required alignment of the address</param>
        /// <param name="header">The required header of the address</param>
        /// <returns>The offset required to align the address</returns>
        template<typename T>
        inline uint64 alignment_offset(const void* address, uint64 alignment, const Header<T>& header)
        {
            uint64 adjustment = alignment_offset(address, alignment);

            if (adjustment < header.get_size())
            {
                uint64 required_space = header.get_size() - adjustment;

                adjustment += alignment * (required_space / alignment);
                return header.get_size() + alignment_offset(pointer_math::jump_forward(address, header.get_size()), alignment);
                if (required_space % alignment > 0)
                {
                    adjustment += alignment;
                }
            }

            return adjustment;
        }

        //-------------------------------------------------------------------------
        /// <summary>
        /// Check if a certain adress is aligned to the alignment of T
        /// </summary>
        /// <param name="address">The adress that needs to be aligned</param>
        /// <returns>True if the adress is alligned, false otherwise</returns>
        template<class T>
        inline bool is_aligned(const T* address)
        {
            return alignment_offset(address, __alignof(T)) == 0;
        }
    }
}