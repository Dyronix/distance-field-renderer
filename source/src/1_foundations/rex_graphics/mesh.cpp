#include "rex_graphics_pch.h"

#include "mesh.h"

#include "datatype.h"
#include "vertex.h"
#include "triangle.h"

#include "resources/resourcefactory.h"
#include "resources/vertexbuffer.h"
#include "resources/indexbuffer.h"

namespace rex
{
    // IMesh
    //-------------------------------------------------------------------------
    rex::BufferLayout rex::IMesh::makeBufferLayout()
    {
        return
        {
            { DataType::VEC3, "a_Position" },
            { DataType::VEC3, "a_Normal" },
            { DataType::VEC3, "a_Tangent" },
            { DataType::VEC3, "a_Binormal" },
            { DataType::VEC2, "a_TexCoord" }
        };
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::VertexBuffer> IMesh::createVertexBuffer(Vertex* vertices, uint32 count, Usage usage, const BufferLayout& layout)
    {
        auto vertex_buffer = ResourceFactory::createVertexBuffer(vertices, sizeof(Vertex) * count, count, usage);

        vertex_buffer->setLayout(layout);

        return vertex_buffer;
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::IndexBuffer> IMesh::createIndexBuffer(TriangleIndices* indices, uint32 count, Usage usage)
    {
        return ResourceFactory::createIndexBuffer(indices, count, usage);
    }

    // Mesh
    //-------------------------------------------------------------------------
    rex::Mesh::Mesh()
        : m_vertex_buffer(nullptr)
        , m_index_buffer(nullptr)
        , m_buffer_layout()
    {

    }
    //-------------------------------------------------------------------------
    rex::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices, Usage usage)
        : m_vertex_buffer(nullptr)
        , m_index_buffer(nullptr)
        , m_buffer_layout(makeBufferLayout())
    {
        m_vertex_buffer = createVertexBuffer(vertices.data(), vertices.size(), usage, m_buffer_layout);
        m_index_buffer = createIndexBuffer(indices.data(), indices.size(), usage);
    }
    //-------------------------------------------------------------------------
    rex::Mesh::~Mesh()
    {
        m_vertex_buffer->release();
        m_index_buffer->release();
    }

    //-------------------------------------------------------------------------
    void rex::Mesh::setVertices(std::vector<Vertex>& vertices)
    {
        m_vertex_buffer.reset();
        m_vertex_buffer = createVertexBuffer(vertices.data(), vertices.size(), getUsage(), m_buffer_layout);
    }
    //-------------------------------------------------------------------------
    void rex::Mesh::setIndices(std::vector<TriangleIndices>& indices)
    {
        m_index_buffer.reset();
        m_index_buffer = createIndexBuffer(indices.data(), indices.size(), getUsage());
    }

    //-------------------------------------------------------------------------
    const ref_ptr<rex::VertexBuffer>& rex::Mesh::getVertexBuffer() const
    {
        return m_vertex_buffer;
    }
    //-------------------------------------------------------------------------
    const ref_ptr<rex::IndexBuffer>& rex::Mesh::getIndexBuffer() const
    {
        return m_index_buffer;
    }

    //-------------------------------------------------------------------------
    uint32 rex::Mesh::getIndexBufferCount() const
    {
        return static_cast<uint32>(m_index_buffer->getCount());
    }
    //-------------------------------------------------------------------------
    uint32 rex::Mesh::getVertexBufferCount() const
    {
        return static_cast<uint32>(m_vertex_buffer->getCount());
    }

    // Static Mesh
    //-------------------------------------------------------------------------
    rex::StaticMesh::StaticMesh()
        :Mesh()
    {

    }
    //-------------------------------------------------------------------------
    rex::StaticMesh::StaticMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices)
        : Mesh(vertices, indices, Usage::STATIC_DRAW)
    {

    }
    //-------------------------------------------------------------------------
    rex::StaticMesh::~StaticMesh()
    {

    }

    //-------------------------------------------------------------------------
    rex::Usage StaticMesh::getUsage() const
    {
        return Usage::STATIC_DRAW;
    }

    // Dynamic Mesh
    //-------------------------------------------------------------------------
    rex::DynamicMesh::DynamicMesh()
        :Mesh()
    {

    }
    //-------------------------------------------------------------------------
    rex::DynamicMesh::DynamicMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices)
        : Mesh(vertices, indices, Usage::DYNAMIC_DRAW)
    {

    }
    //-------------------------------------------------------------------------
    rex::DynamicMesh::~DynamicMesh()
    {

    }

    //-------------------------------------------------------------------------
    rex::Usage DynamicMesh::getUsage() const
    {
        return Usage::DYNAMIC_DRAW;
    }
}
