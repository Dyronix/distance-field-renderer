#include "rex_memory_pch.h"

#include "blobreader.h"

namespace rex
{
    namespace memory
    {
        //-------------------------------------------------------------------------
        BlobReader::BlobReader(const memory::Blob& b)
            :blob(b)
            , read_offset(0)
        {}
        //-------------------------------------------------------------------------
        BlobReader::BlobReader(const memory::Blob& b, size_t offset)
            :blob(b)
            , read_offset(offset)
        {}

        //-------------------------------------------------------------------------
        memory::byte* BlobReader::read(const memory::size& bytesToRead)
        {
            memory::byte* value = blob.read_bytes(bytesToRead, read_offset);
            read_offset += sizeof(bytesToRead);
            return value;
        }

        namespace reader
        {

            //-------------------------------------------------------------------------
            rex::memory::byte* read(const memory::Blob& b, const memory::size& bytesToRead, const memory::size& offset)
            {
                BlobReader reader(b, offset);
                return reader.read(bytesToRead);
            }

        }
    }
}