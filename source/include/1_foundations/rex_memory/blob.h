#pragma once

#include "memory_size.h"

namespace rex
{
    namespace memory
    {
        class Blob
        {
        public:
            static void copy(const Blob& src, Blob& dst);
            static void copy(const Blob& src, void* dst);
            static void copy(void* src, const memory::size& size, Blob& dst);

            REX_NODISCARD static Blob copy(const void* data, const memory::size& size);

            Blob();
            Blob(std::nullptr_t n);
            Blob(std::unique_ptr<memory::byte> fileData, const memory::size& fileSize);

            Blob(const Blob& other);
            Blob(Blob&& other) noexcept;

            Blob& operator=(const Blob& other);
            Blob& operator=(Blob&& other) noexcept;
            Blob& operator=(std::nullptr_t n) noexcept;

            operator bool() const;

            memory::byte& operator[](int index);
            const memory::byte& operator[](int index) const;

            void allocate(const memory::size& inSize);
            void release();
            void zero_initialize();

            memory::byte* get_data();
            const memory::byte* get_data() const;

            const memory::size& get_size() const;

            template <typename T>
            T* get_data_as();
            template <typename T>
            const T* get_data_as() const;

        private:
            friend struct BlobWriter;
            friend struct BlobReader;

            template <typename T>
            T& read(const memory::size& offset = 0) const;
            memory::byte* read_bytes(const memory::size& inSize, const memory::size& inOffset) const;

            void write(const void* inData, const memory::size& inSize, const memory::size& inOffset = 0);

            std::unique_ptr<memory::byte> m_data;
            memory::size m_size;
        };

        //-------------------------------------------------------------------------
        Blob make_blob(const memory::byte* inData, const memory::size& inSize);

        //-------------------------------------------------------------------------
        template <typename T>
        T& Blob::read(const memory::size& offset) const
        {
            return *(T*)((memory::byte*)m_data.get() + offset);
        }

        //-------------------------------------------------------------------------
        template <typename T>
        T* Blob::get_data_as()
        {
            return reinterpret_cast<T*>(m_data.get());
        }
        //-------------------------------------------------------------------------
        template <typename T>
        const T* Blob::get_data_as() const
        {
            return reinterpret_cast<const T*>(m_data.get());
        }
    }
}