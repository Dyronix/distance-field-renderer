#include "rex_memory_pch.h"

#include "blob.h"
#include "blobwriter.h"

namespace rex
{
    namespace memory
    {
        //-------------------------------------------------------------------------
        void Blob::copy(const Blob& src, Blob& dst)
        {
            if (src.m_size == 0)
            {
                return;
            }

            if (dst.m_size == 0)
            {
                dst.allocate(src.m_size);
                dst.zero_initialize();
            }

            R_ASSERT(dst.m_size >= src.m_size);

            memcpy(dst.m_data.get(), src.m_data.get(), src.m_size);
        }
        //-------------------------------------------------------------------------
        void Blob::copy(const Blob& src, void* dst)
        {
            if (src.m_size == 0)
            {
                return;
            }

            R_ASSERT(dst != nullptr);

            memcpy(dst, src.m_data.get(), src.m_size);
        }
        //-------------------------------------------------------------------------
        void Blob::copy(void* src, const memory::size& size, Blob& dst)
        {
            R_ASSERT(src != nullptr);
            R_ASSERT(size != 0);

            if (dst.m_size == 0)
            {
                dst.allocate(size);
                dst.zero_initialize();
            }

            R_ASSERT(dst.m_size >= size);

            memcpy(dst.m_data.get(), src, size);
        }

        //-------------------------------------------------------------------------
        REX_NODISCARD Blob Blob::copy(const void* data, const memory::size& size)
        {
            Blob blob;
            blob.allocate(size);

            memcpy(blob.m_data.get(), data, size);

            return blob;
        }

        //-------------------------------------------------------------------------
        Blob::Blob()
            : m_data(nullptr)
            , m_size(0_bytes)
        {
        }
        //-------------------------------------------------------------------------
        Blob::Blob(std::nullptr_t n)
            : m_data(n)
            , m_size(0_bytes)
        {
        }
        //-------------------------------------------------------------------------
        Blob::Blob(std::unique_ptr<memory::byte> fileData, const memory::size& fileSize)
            : m_data(std::exchange(fileData, nullptr))
            , m_size(fileSize)
        {
        }

        //-------------------------------------------------------------------------
        Blob::Blob(const Blob& other)
        {
            if (m_data)
            {
                release();
            }

            allocate(other.m_size);
            zero_initialize();

            memcpy(m_data.get(), other.m_data.get(), m_size);
        }
        //-------------------------------------------------------------------------
        Blob::Blob(Blob&& other) noexcept 
            : m_data(std::exchange(other.m_data, nullptr)), m_size(std::exchange(other.m_size, 0))
        {
        }

        //-------------------------------------------------------------------------
        Blob& Blob::operator=(const Blob& other)
        {
            // Guard self assignment
            if (this == &other)
            {
                return *this;
            }

            if (m_data)
            {
                release();
            }

            allocate(other.m_size);
            zero_initialize();

            memcpy(m_data.get(), other.m_data.get(), m_size);

            return *this;
        }
        //-------------------------------------------------------------------------
        Blob& Blob::operator=(Blob&& other) noexcept
        {
            // Guard self assignment
            if (this == &other)
            {
                return *this;
            }

            m_data = std::exchange(other.m_data, nullptr);
            m_size = std::exchange(other.m_size, 0);

            return *this;
        }
        //-------------------------------------------------------------------------
        Blob& Blob::operator=(std::nullptr_t n) noexcept
        {
            release();

            m_data = n;
            m_size = 0;

            return *this;
        }

        //-------------------------------------------------------------------------
        Blob::operator bool() const
        {
            return m_data != nullptr;
        }

        //-------------------------------------------------------------------------
        memory::byte& Blob::operator[](int index)
        {
            return ((byte*)m_data.get())[index];
        }
        //-------------------------------------------------------------------------
        const memory::byte& Blob::operator[](int index) const
        {
            return ((byte*)m_data.get())[index];
        }

        //-------------------------------------------------------------------------
        void Blob::allocate(const memory::size& inSize)
        {
            release();

            if (inSize == 0)
            {
                return;
            }

            m_data = std::unique_ptr<memory::byte>(new memory::byte[inSize]);
            m_size = inSize;
        }
        //-------------------------------------------------------------------------
        void Blob::release()
        {
            m_data.reset();
            m_size = 0;
        }
        //-------------------------------------------------------------------------
        void Blob::zero_initialize()
        {
            if (m_data)
            {
                std::memset(m_data.get(), 0, m_size);
            }
        }

        //-------------------------------------------------------------------------
        memory::byte* Blob::read_bytes(const memory::size& inSize, const memory::size& inOffset) const
        {
            R_ASSERT_X(inOffset + inSize <= m_size, "Buffer overflow!");

            byte* buffer = new byte[inSize];
            memcpy(buffer, (byte*)m_data.get() + inOffset, inSize);
            return buffer;
        }

        //-------------------------------------------------------------------------
        void Blob::write(const void* inData, const memory::size& inSize, const memory::size& inOffset)
        {
            R_ASSERT_X(inOffset + inSize <= m_size, "Buffer overflow!");

            memcpy((byte*)m_data.get() + inOffset, inData, inSize);
        }

        //-------------------------------------------------------------------------
        memory::byte* Blob::get_data()
        {
            return m_data.get();
        }
        //-------------------------------------------------------------------------
        const memory::byte* Blob::get_data() const
        {
            return m_data.get();
        }

        //-------------------------------------------------------------------------
        const memory::size& Blob::get_size() const
        {
            return m_size;
        }

        //-------------------------------------------------------------------------
        Blob make_blob(const memory::byte* inData, const memory::size& inSize)
        {
            Blob blob;

            blob.allocate(inSize);
            blob.zero_initialize();

            if (inData != nullptr)
            {
                writer::write(blob, inData, inSize);
            }

            return blob;
        }
    }
}