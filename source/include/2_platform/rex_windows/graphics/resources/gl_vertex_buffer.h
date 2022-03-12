#pragma once

#include "buffer_usage.h"
#include "blob.h"

#include "resources/vertex_buffer.h"

namespace rex
{
    struct Vertex;

    namespace opengl
    {
        class VertexBuffer : public rex::VertexBuffer
        {
        public:
            VertexBuffer(void* vertices, uint32 size, uint32 count, BufferUsage usage = BufferUsage::STATIC_DRAW);
            virtual ~VertexBuffer() override;

            void            bind() const override;
            void            unbind() const override;

            void            release() override;

            uint32          get_count() override;
            const uint32    get_count() const override;

            Layout&         get_layout() override;
            const Layout&   get_layout() const override;

            void            set_layout(const Layout& layout) override;
            
        private:
            uint32          m_count;
            uint32          m_buffer_id;
            Layout          m_layout;
            memory::Blob    m_local_storate;
            BufferUsage           m_usage;
        };
    }
}