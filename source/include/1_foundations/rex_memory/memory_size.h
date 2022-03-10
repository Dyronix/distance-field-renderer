#pragma once

#include <ratio>
#include <utility>

namespace rex
{
    namespace memory
    {
        class size
        {
        public:
            static constexpr uint16 BYTES_IN_KIBIBYTES = 1 << 10;  // 1024
            static constexpr uint32 BYTES_IN_MEBIBYTES = 1 << 20;  // 1'048'576
            static constexpr uint64 BYTES_IN_GIBIBYTES = 1 << 30;  // 1'073'741'824

            size() noexcept;
            size(const size& other) noexcept;
            size(size&& other) noexcept;
            size(uint64 byteCount) noexcept;
            ~size();

            size& operator=(const size& other) noexcept;
            size& operator=(size&& other) noexcept;

            uint64 bytes() const noexcept;
            uint64 kilo_bytes() const noexcept;
            uint64 kibi_bytes() const noexcept;
            uint64 mega_bytes() const noexcept;
            uint64 mebi_bytes() const noexcept;
            uint64 giga_bytes() const noexcept;
            uint64 gibi_bytes() const noexcept;

            operator uint64() const noexcept;

            size operator+(uint64 other) const;
            size operator-(uint64 other) const;
            size operator+(const size& other) const;
            size operator-(const size& other) const;

            uint64* get_data();

        private:
            uint64 m_size_in_bytes;
        };
    }
}

//-------------------------------------------------------------------------
inline rex::memory::size operator"" _bytes(uint64 bytes)
{
    return rex::memory::size{bytes};
}
//-------------------------------------------------------------------------
inline rex::memory::size operator"" _kb(uint64 kilobytes)
{
    return rex::memory::size{kilobytes * std::kilo::num};
}
//-------------------------------------------------------------------------
inline rex::memory::size operator"" _kib(uint64 kibibytes)
{
    return rex::memory::size{kibibytes * rex::memory::size::BYTES_IN_KIBIBYTES};
}
//-------------------------------------------------------------------------
inline rex::memory::size operator"" _mb(uint64 megabytes)
{
    return rex::memory::size{megabytes * std::mega::num};
}
//-------------------------------------------------------------------------
inline rex::memory::size operator"" _mib(uint64 mebibytes)
{
    return rex::memory::size{mebibytes * rex::memory::size::BYTES_IN_MEBIBYTES};
}
//-------------------------------------------------------------------------
inline rex::memory::size operator"" _gb(uint64 gigabytes)
{
    return rex::memory::size{gigabytes * std::giga::num};
}
//-------------------------------------------------------------------------
inline rex::memory::size operator"" _gib(uint64 gibibytes)
{
    return rex::memory::size{gibibytes * rex::memory::size::BYTES_IN_GIBIBYTES};
}