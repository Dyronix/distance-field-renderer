#include "rex_windows_pch.h"

#include "graphics/resources/gl_index_buffer.h"

#include "renderer/renderer.h"

#include "triangle.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        IndexBuffer::IndexBuffer(TriangleIndices* indices, uint32 count, BufferUsage usage)
            : m_count(count * TriangleIndices::INDICES_PER_TRIANGLE)
            , m_triangle_count(count)
            , m_buffer_id(0)
            , m_indices(indices)
            , m_local_storate(memory::Blob::copy(indices, (sizeof(TriangleIndices) * count)))
        {
            RENDERER_INFO("Submitting - Creating Index Buffer");

            ref_ptr<IndexBuffer> instance(this);
            Renderer::submit([instance, usage]() mutable
                {
                    RENDERER_INFO("Executing - Creating Index Buffer");

                    opengl::generate_buffers(1, &instance->m_buffer_id);
                    opengl::bind_buffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_buffer_id);
                    opengl::buffer_data(GL_ELEMENT_ARRAY_BUFFER, gsl::narrow<uint32>(instance->m_local_storate.get_size()), instance->m_local_storate.get_data_as<void>(), usage == BufferUsage::STATIC_DRAW ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
                });
        }

        //-------------------------------------------------------------------------
        IndexBuffer::~IndexBuffer()
        {
            if (m_buffer_id)
            {
                ref_ptr<IndexBuffer> instance(this);
                Renderer::submit([instance]() mutable
                    {
                        opengl::delete_buffers(1, &instance->m_buffer_id);
                        instance->m_buffer_id = 0;
                    });
            }
        }

        //-------------------------------------------------------------------------
        void IndexBuffer::bind() const
        {
            RENDERER_INFO("Submitting - Bind Index Buffer");

            ref_ptr<const IndexBuffer> instance(this);
            Renderer::submit([instance]()
                {
                    RENDERER_INFO("Executing - Bind Index Buffer");

                    opengl::bind_buffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_buffer_id);
                });
        }

        //-------------------------------------------------------------------------
        void IndexBuffer::unbind() const
        {
            RENDERER_INFO("Submitting - Unbind Index Buffer");

            ref_ptr<const IndexBuffer> instance(this);
            Renderer::submit([instance]()
                {
                    RENDERER_INFO("Executing - Unbind Index Buffer");

                    opengl::bind_buffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                });
        }

        //-------------------------------------------------------------------------
        void IndexBuffer::release()
        {
            if (m_buffer_id)
            {
                opengl::delete_buffers(1, &m_buffer_id);
                m_buffer_id = 0;
            }
        }

        //-------------------------------------------------------------------------
        void* IndexBuffer::get_indices()
        {
            return m_indices;
        }

        //-------------------------------------------------------------------------
        const void* IndexBuffer::get_indices() const
        {
            return m_indices;
        }

        //-------------------------------------------------------------------------
        uint32 IndexBuffer::get_count()
        {
            return m_count;
        }
        //-------------------------------------------------------------------------
        const uint32 IndexBuffer::get_count() const
        {
            return m_count;
        }

        //-------------------------------------------------------------------------
        uint32 IndexBuffer::get_triangle_count()
        {
            return m_triangle_count;
        }
        //-------------------------------------------------------------------------
        const uint32 IndexBuffer::get_triangle_count() const
        {
            return m_triangle_count;
        }

    }
}
