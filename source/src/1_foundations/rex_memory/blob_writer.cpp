#include "rex_memory_pch.h"

#include "blobwriter.h"

namespace rex
{
    namespace memory
    {
        //-------------------------------------------------------------------------
        BlobWriter::BlobWriter(const BlobWriter& other)
            : blob(other.blob)
            , write_offset(other.write_offset)
        {
        }
        //-------------------------------------------------------------------------
        BlobWriter::BlobWriter(memory::Blob& b)
            : blob(b)
            , write_offset(0)
        {
        }
        //-------------------------------------------------------------------------
        BlobWriter::BlobWriter(memory::Blob& b, size_t offset)
            : blob(b)
            , write_offset(offset)
        {
        }

        //-------------------------------------------------------------------------
        BlobWriter& BlobWriter::operator=(const BlobWriter& other)
        {
            blob = other.blob;
            write_offset = other.write_offset;

            return *this;
        }

        //-------------------------------------------------------------------------
        void BlobWriter::write(const void* inData, const memory::size& inSize)
        {
            blob.write(inData, inSize, write_offset);
            write_offset += inSize;
        }

        //-------------------------------------------------------------------------
        size_t BlobWriter::get_write_offset() const
        {
            return write_offset;
        }

        namespace writer
        {
            //-------------------------------------------------------------------------
            void write(memory::Blob& b, const void* inData, const memory::size& inSize)
            {
                BlobWriter writer(b);
                writer.write(inData, inSize);
            }

            //-------------------------------------------------------------------------
            void write(memory::Blob& b, const void* inData, const memory::size& inSize, const memory::size& inOffset)
            {
                BlobWriter writer(b, inOffset);
                writer.write(inData, inSize);
            }
        }
    }
}