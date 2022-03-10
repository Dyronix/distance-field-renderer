#pragma once

#include "blob.h"

namespace rex
{
    namespace memory
    {
        struct BlobWriter
        {
            BlobWriter(const BlobWriter& other);
            BlobWriter(memory::Blob& b);
            BlobWriter(memory::Blob& b, size_t offset);

            BlobWriter& operator=(const BlobWriter& other);

            template <typename T>
            void write(const T& data);
            void write(const void* inData, const memory::size& inSize);

            size_t get_write_offset() const;

            memory::Blob& blob;
            size_t write_offset;
        };

        //-------------------------------------------------------------------------
        template <typename T>
        void rex::memory::BlobWriter::write(const T& data)
        {
            blob.write(&data, sizeof(T), write_offset);
            write_offset += sizeof(T);
        }

        namespace writer
        {
            //-------------------------------------------------------------------------
            template <typename T>
            inline void write(memory::Blob& b, const T& data)
            {
                BlobWriter writer(b);

                writer.template write<T>(data);
            }

            void write(memory::Blob& b, const void* inData, const memory::size& inSize);
            void write(memory::Blob& b, const void* inData, const memory::size& inSize, const memory::size& inOffset);
        }
    }
}