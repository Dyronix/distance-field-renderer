#pragma once

#include "buffer_usage.h"

#include "blob.h"

#include "resources/index_buffer.h"

namespace rex
{
    struct TriangleIndices;

    namespace opengl
    {
        class IndexBuffer : public rex::IndexBuffer
        {
        public:
            IndexBuffer(TriangleIndices* indices, uint32 count, BufferUsage usage = BufferUsage::STATIC_DRAW);
            ~IndexBuffer();

            void bind() const override;
            void unbind() const override;

            void release() override;

            void* get_indices() override;
            const void* get_indices() const override;

            uint32 get_count() override;
            const uint32 get_count() const override;

            uint32 get_triangle_count() override;
            const uint32 get_triangle_count() const override;

        private:
            uint32 m_buffer_id;
            TriangleIndices* m_indices;
            uint32 m_count;
            uint32 m_triangle_count;
            memory::Blob m_local_storate;
        };
    }
}