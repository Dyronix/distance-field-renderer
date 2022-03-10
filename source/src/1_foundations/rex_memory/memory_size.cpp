#include "rex_memory_pch.h"

#include "memory_size.h"

namespace rex
{
    namespace memory
    {
        //-------------------------------------------------------------------------
        size::size() noexcept
            :m_size_in_bytes(0)
        {}

        //-------------------------------------------------------------------------
        size::size(uint64 byteCount) noexcept
            :m_size_in_bytes(byteCount)
        {}

        //-------------------------------------------------------------------------
        size::size(const size& other) noexcept
            :m_size_in_bytes(other.m_size_in_bytes)
        {}

        //-------------------------------------------------------------------------
        size::size(size&& other) noexcept
            :m_size_in_bytes(std::exchange(other.m_size_in_bytes, 0))
        {}

        //-------------------------------------------------------------------------
        size::~size()
        {}

        //-------------------------------------------------------------------------
        size& size::operator=(const size& other) noexcept
        {
            m_size_in_bytes = other.m_size_in_bytes;

            return *this;
        }

        //-------------------------------------------------------------------------
        size& size::operator=(size&& other) noexcept
        {
            m_size_in_bytes = std::exchange(other.m_size_in_bytes, 0);

            return *this;
        }

        //-------------------------------------------------------------------------
        uint64 size::bytes() const noexcept
        {
            return m_size_in_bytes;
        }
        //-------------------------------------------------------------------------
        uint64 size::kilo_bytes() const noexcept
        {
            return m_size_in_bytes / std::kilo::num;
        }
        //-------------------------------------------------------------------------
        uint64 size::kibi_bytes() const noexcept
        {
            return m_size_in_bytes / BYTES_IN_KIBIBYTES;
        }
        //-------------------------------------------------------------------------
        uint64 size::mega_bytes() const noexcept
        {
            return m_size_in_bytes / std::mega::num;
        }
        //-------------------------------------------------------------------------
        uint64 size::mebi_bytes() const noexcept
        {
            return m_size_in_bytes / BYTES_IN_MEBIBYTES;
        }
        //-------------------------------------------------------------------------
        uint64 size::giga_bytes() const noexcept
        {
            return m_size_in_bytes / std::giga::num;
        }
        //-------------------------------------------------------------------------
        uint64 size::gibi_bytes() const noexcept
        {
            return m_size_in_bytes / BYTES_IN_GIBIBYTES;
        }

        //-------------------------------------------------------------------------
        size::operator uint64() const noexcept
        {
            return m_size_in_bytes;
        }

        //-------------------------------------------------------------------------
        size size::operator+ (uint64 other) const
        {
            return size(m_size_in_bytes + other);
        }
        //-------------------------------------------------------------------------
        size size::operator- (uint64 other) const
        {
            return size(m_size_in_bytes - other);
        }
        //-------------------------------------------------------------------------
        size size::operator+(const size& other) const
        {
            return size(m_size_in_bytes + other.m_size_in_bytes);
        }
        //-------------------------------------------------------------------------
        size size::operator-(const size& other) const
        {
            return size(m_size_in_bytes - other.m_size_in_bytes);
        }

        //-------------------------------------------------------------------------
        uint64* size::get_data()
        {
            return &m_size_in_bytes;
        }
    }
}