#pragma once

#include "resources/api_resource.h"

namespace rex
{
    class VertexBuffer;

    namespace opengl
    {
        class VertexArray : public Resource
        {
        public:
            using VertexBuffers = std::vector<rex::VertexBuffer*>;

            VertexArray();
            ~VertexArray() override;

            void                    release() override;

            void                    bind() const;
            void                    unbind() const;

            void                    add_vertex_buffer(VertexBuffer* vertexBuffer);
            void                    set_index_buffer(IndexBuffer* indexBuffer);

            VertexBuffers&          get_vertex_buffers();
            const VertexBuffers&    get_vertex_buffers() const;

            IndexBuffer*            get_index_buffer();
            const IndexBuffer*      get_index_buffer() const;

        private:
            unsigned int            m_array_id;
            VertexBuffers           m_vertex_buffers;
            IndexBuffer*            m_index_buffer;
        };
    }
}