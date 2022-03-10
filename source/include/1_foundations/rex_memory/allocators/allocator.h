#pragma once

#include "architecture.h"

#include "blob.h"

namespace rex
{
    namespace memory
    {
        struct MemoryHeader
        {
            //-------------------------------------------------------------------------
            MemoryHeader()
                :blob()
                ,alignment(0)
            {}
            //-------------------------------------------------------------------------
            MemoryHeader(size s, void* m, uint8 a)
                :blob(make_blob((memory::byte*)m, s))
                ,alignment(a)
            {}

            Blob blob;
            uint8 alignment;
        };

        /// <summary>
        /// Representation of an interface of an allocator
        /// </summary>
        class Allocator
        {
        public:
            /// <summary>
            /// Ctor of the base allocator
            /// All allocators should implement the interface of this class
            /// </summary>
            /// <param name="block">The memory we should control using this allocator</param>
            explicit Allocator(const MemoryHeader& block);
            /// <summary>
            /// The Dtor of the base allocator
            /// </summary>
            virtual ~Allocator();

            /// <summary>
            /// Allocate new memory
            /// </summary>
            /// <param name="size">The size we would like to allocate</param>
            /// <param name="alignment">The alignment of the returned memory adress</param>
            /// <returns>A pointer to the allocated memory</returns>
            virtual void*       allocate(const size& size) = 0;
            /// <summary>
            /// Deallocate memory
            /// </summary>
            /// <param name="ptr">The pointer to the memory we would like to deallocate</param>
            virtual void        deallocate(void* ptr) = 0;

            /// <summary>
            /// Clear all memory inside this allocator
            /// </summary>
            virtual void        clear();

            /// <summary>
            /// Free all memory inside this allocator
            /// </summary>
            virtual void        free();

            /// <summary>
            /// Retrieve the base pointer of this allocator ( a.k.a.: The start of the allocated memory )
            /// </summary>
            /// <returns>The start of the allocated memory</returns>
                    void*       get_base_pointer();

            /// <summary>
            /// Retrieve the base pointer of this allocator ( a.k.a.: The start of the allocated memory )
            /// </summary>
            /// <returns>The start of the allocated memory</returns>
                    const void* get_base_pointer() const;

            /// <summary>
            /// Retrieve the aligned base pointer of this allocator ( a.k.a.: The start of the stored data )
            /// </summary>
            /// <returns>The start of store data</returns>
                          void* get_aligned_base_pointer();

            /// <summary>
            /// Retrieve the aligned base pointer of this allocator ( a.k.a.: The start of the stored data )
            /// </summary>
            /// <returns>The start of store data</returns>
                    const void* get_aligned_base_pointer() const;

            /// <summary>
            /// Retrieve the total amount of memory we could allocate using this allocator
            /// </summary>
            /// <returns>The total amount of memory we could allocate</returns>
                    size        get_total_size() const;
            /// <summary>
            /// Retrieve the total amount of used memory we allocated using this allocator
            /// </summary>
            /// <returns>The used amount of memory we allocated</returns>
            virtual size        get_used_size() const = 0;

            /// <summary>
            /// Retrieve the total amount of allocations we required using this allocator
            /// </summary>
            /// <returns>The total amount of allocations</returns>
            virtual uint64      get_number_of_allocations() const = 0;

            /// <summary>
            /// Retrieve the amount of bits this allocator will align to
            /// </summary>
            /// <returns>The alignment</returns>
                    uint64      get_alignment() const;

        protected:
            /// <summary>
            /// Clear all memory inside this allocator
            /// </summary>
            virtual void        on_clear() { /* To be implemented in derived */ }
            /// <summary>
            /// Free all memory inside this allocator
            /// </summary>
            virtual void        on_free() { /* To be implemented in derived */ }

            /// <summary>
            /// Function to test if we can allocate new memory
            /// </summary>
            /// <param name="size">The size of the memory we would like to allocate</param>
            /// <returns>Returns true if allowed, otherwise false if we cannot allocate any more memory.</returns>
                    bool        can_allocate(const size& size) const;

            /// <summary>
            /// A reference to the memory block we control inside this allocator
            /// </summary>
            /// <returns>The memory block</returns>
            const MemoryHeader& get_memory() const;

        private:
            /// <summary>
            /// Internal memory block we control using this allocator
            /// </summary>
            MemoryHeader m_memory_block;
        };

        //-------------------------------------------------------------------------
        template<typename T>
        T create_allocator(size size)
        {
            void* raw_memory = malloc(size.bytes());
            memset(raw_memory, 0, size.bytes());

            MemoryHeader h =
            {
                size,                                                           // The size of the allocated memory
                raw_memory,                                                     // A pointer to the allocated memory
                configuration::is_32_architecture ? (uint8)32u : (uint8)64u     // We would like our memory to be aligned on this amount of bits
            };

            return T(h);
        }
    }
}