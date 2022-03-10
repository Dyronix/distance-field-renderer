#pragma once

#include "blob.h"

namespace rex
{
    namespace memory
    {
        struct BlobReader
        {
            BlobReader(const memory::Blob& b);
            BlobReader(const memory::Blob& b, size_t offset);

            template <typename T>
            T read();
            memory::byte* read(const memory::size& bytesToRead);

            const memory::Blob& blob;
            size_t read_offset;
        };

        //-------------------------------------------------------------------------
        template <typename T>
        T rex::memory::BlobReader::read()
        {
            T value = blob.template read<T>(read_offset);
            read_offset += sizeof(T);
            return value;
        }

        namespace reader
        {
            //-------------------------------------------------------------------------
            template <typename T>
            inline T read(const memory::Blob& b)
            {
                BlobReader reader(b);
                return reader.template read<T>();
            }
            //-------------------------------------------------------------------------
            template <typename T>
            inline T read(const memory::Blob& b, const memory::size& offset)
            {
                BlobReader reader(b, offset);
                return reader.template read<T>();
            }

            //-------------------------------------------------------------------------
            memory::byte* read(const memory::Blob& b, const memory::size& bytesToRead, const memory::size& offset);
        }
    }
}