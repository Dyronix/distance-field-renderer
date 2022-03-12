#include "rex_windows_pch.h"

#include "data_type.h"

#include "resources/buffer_layout.h"

#include "graphics/resources/gl_index_buffer.h"
#include "graphics/resources/gl_vertex_buffer.h"
#include "graphics/resources/gl_vertex_array.h"

namespace rex
{
    namespace opengl
    {
        namespace vertex_array
        {
            //-------------------------------------------------------------------------
            void enable_vertex_attribute_array(uint32 index)
            {
                opengl::enable_vertex_attrib_array(index);
            }

            //-------------------------------------------------------------------------
            void define_attribute_data_int(uint32 index, const BufferElement& element, uint32 stride)
            {
                opengl::vertex_attrib_i_pointer(
                    index,                                      // Specifies the index of the generic vertex attribute to be modified.
                    element.component_count,                    // Specifies the number of components per generic vertex attribute.
                    to_open_gl_data_type(element.type),             // Specifies the data type of each component in the array.
                    stride,                                     // Specifies the byte offset between consecutive generic vertex attributes. 
#pragma warning( push )
#pragma warning( disable : 4312 )
                    (const void*)element.offset);               // Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
#pragma warning( pop )
            }
            //-------------------------------------------------------------------------
            void define_vertex_attribute_data(uint32 index, const BufferElement& element, uint32 stride)
            {
                opengl::vertex_attrib_pointer(
                    index,                                      // Specifies the index of the generic vertex attribute to be modified.
                    element.component_count,                    // Specifies the number of components per generic vertex attribute.
                    to_open_gl_data_type(element.type),             // Specifies the data type of each component in the array.
                    element.normalized ? GL_TRUE : GL_FALSE,    // Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
                    stride,                                     // Specifies the byte offset between consecutive generic vertex attributes. 
#pragma warning( push )
#pragma warning( disable : 4312 )
                    (const void*)element.offset);               // Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
#pragma warning( pop )
            }
        }

        //-------------------------------------------------------------------------
        VertexArray::VertexArray()
            :m_index_buffer(nullptr)
        {
            opengl::generate_vertex_arrays(1, &m_array_id);
        }

        //-------------------------------------------------------------------------
        VertexArray::~VertexArray()
        {
            opengl::delete_vertex_arrays(1, &m_array_id);
            m_array_id = 0;
        }

        //-------------------------------------------------------------------------
        void VertexArray::release()
        {
            opengl::delete_vertex_arrays(1, &m_array_id);
            m_array_id = 0;
        }

        //-------------------------------------------------------------------------
        void VertexArray::bind() const
        {
            opengl::bind_vertex_array(m_array_id);
        }

        //-------------------------------------------------------------------------
        void VertexArray::unbind() const
        {
            opengl::bind_vertex_array(0);
        }

        //-------------------------------------------------------------------------
        void VertexArray::add_vertex_buffer(VertexBuffer* vertexBuffer)
        {
            R_ASSERT_X(vertexBuffer->get_layout().get_size(), "Vertex Buffer has no layout!");

            bind();
            vertexBuffer->bind();

            const BufferLayout& layout = vertexBuffer->get_layout();
            for(uint32 i = 0; i < layout.get_size(); ++i)
            {
                vertex_array::enable_vertex_attribute_array(i);
                vertex_array::define_vertex_attribute_data(i, layout[i], layout.get_stride());
            }

            m_vertex_buffers.push_back(vertexBuffer);

            vertexBuffer->unbind();
            unbind();
        }

        //-------------------------------------------------------------------------
        void VertexArray::set_index_buffer(IndexBuffer* indexBuffer)
        {
            bind();
            indexBuffer->bind();

            m_index_buffer = indexBuffer;

            indexBuffer->unbind();
            unbind();
        }

        //-------------------------------------------------------------------------
        VertexArray::VertexBuffers& VertexArray::get_vertex_buffers()
        {
            return m_vertex_buffers;
        }

        //-------------------------------------------------------------------------
        const VertexArray::VertexBuffers& VertexArray::get_vertex_buffers() const
        {
            return m_vertex_buffers;
        }

        //-------------------------------------------------------------------------
        IndexBuffer* VertexArray::get_index_buffer()
        {
            return m_index_buffer;     
        }

        //-------------------------------------------------------------------------
        const IndexBuffer* VertexArray::get_index_buffer() const
        {
            return m_index_buffer;
        }
    }
}
