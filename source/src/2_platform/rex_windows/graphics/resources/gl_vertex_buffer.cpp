#include "rex_windows_pch.h"

#include "graphics/resources/gl_vertex_buffer.h"

#include "renderer/renderer.h"

#include "mesh.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        VertexBuffer::VertexBuffer(void* vertices, uint32 size, uint32 count, BufferUsage usage)
            : m_count(count)
            , m_usage(usage)
            , m_local_storate(memory::Blob::copy(vertices, size))
            , m_buffer_id(0)
        {
            RENDERER_INFO("Submitting - Creating Vertex Buffer");

            ref_ptr<VertexBuffer> instance(this);
            Renderer::submit([instance]() mutable
                             {
                                 RENDERER_INFO("Executing - Creating Vertex Buffer");

                                 auto usage = instance->m_usage == BufferUsage::STATIC_DRAW ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

                                 opengl::generate_buffers(1, &instance->m_buffer_id);
                                 opengl::bind_buffer(GL_ARRAY_BUFFER, instance->m_buffer_id);
                                 opengl::buffer_data(GL_ARRAY_BUFFER, gsl::narrow<uint32>(instance->m_local_storate.get_size()), instance->m_local_storate.get_data_as<void>(), usage);
                             });
        }

        //-------------------------------------------------------------------------
        VertexBuffer::~VertexBuffer()
        {
            if (m_buffer_id)
            {
                ref_ptr<VertexBuffer> instance(this);
                Renderer::submit([instance]() mutable
                                 {
                                     opengl::delete_buffers(1, &instance->m_buffer_id);
                                     instance->m_buffer_id = 0;
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void VertexBuffer::bind() const
        {
            RENDERER_INFO("Submitting - Bind Vertex Buffer");

            ref_ptr<const VertexBuffer> instance(this);
            Renderer::submit([instance]()
                             {
                                 RENDERER_INFO("Executing - Bind Vertex Buffer");

                                 opengl::bind_buffer(GL_ARRAY_BUFFER, instance->m_buffer_id);
                             });
        }

        //-------------------------------------------------------------------------
        void VertexBuffer::unbind() const
        {
            RENDERER_INFO("Submitting - Unbind Vertex Buffer");

            ref_ptr<const VertexBuffer> instance(this);
            Renderer::submit([instance]()
                             {
                                 RENDERER_INFO("Executing - Unbind Vertex Buffer");

                                 opengl::bind_buffer(GL_ARRAY_BUFFER, 0);
                             });
        }

        //-------------------------------------------------------------------------
        void VertexBuffer::release()
        {
            if (m_buffer_id)
            {
                opengl::delete_buffers(1, &m_buffer_id);
                m_buffer_id = 0;
            }
        }

        //-------------------------------------------------------------------------
        uint32 VertexBuffer::get_count()
        {
            return m_count;
        }

        //-------------------------------------------------------------------------
        const uint32 VertexBuffer::get_count() const
        {
            return m_count;
        }

        //-------------------------------------------------------------------------
        Layout& VertexBuffer::get_layout()
        {
            return m_layout;
        }

        //-------------------------------------------------------------------------
        const Layout& VertexBuffer::get_layout() const
        {
            return m_layout;
        }

        //-------------------------------------------------------------------------
        void VertexBuffer::set_layout(const Layout& layout)
        {
            m_layout = layout;
        }
    }
}