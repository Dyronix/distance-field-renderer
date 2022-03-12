#include "rex_graphics_pch.h"

#include "mesh.h"

#include "buffer_usage.h"
#include "data_type.h"
#include "vertex.h"
#include "triangle.h"

#include "resources/resource_factory.h"
#include "resources/vertex_buffer.h"
#include "resources/index_buffer.h"

namespace rex
{
    // IMesh
    //-------------------------------------------------------------------------
    rex::BufferLayout rex::IMesh::make_buffer_layout()
    {
        return {{DataType::VEC3, "a_Position"},
                {DataType::VEC3, "a_Normal"},
                {DataType::VEC3, "a_Tangent"},
                {DataType::VEC3, "a_Binormal"},
                {DataType::VEC2, "a_TexCoord"}};
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::VertexBuffer> IMesh::create_vertex_buffer(Vertex* vertices, uint32 count, BufferUsage usage, const BufferLayout& layout)
    {
        auto vertex_buffer = ResourceFactory::create_vertex_buffer(vertices, sizeof(Vertex) * count, count, usage);

        vertex_buffer->set_layout(layout);

        return vertex_buffer;
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::IndexBuffer> IMesh::create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage)
    {
        return ResourceFactory::create_index_buffer(indices, count, usage);
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
    rex::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices, BufferUsage usage)
        : m_vertex_buffer(nullptr)
        , m_index_buffer(nullptr)
        , m_buffer_layout(make_buffer_layout())
    {
        m_vertex_buffer = create_vertex_buffer(vertices.data(), gsl::narrow<uint32>(vertices.size()), usage, m_buffer_layout);
        m_index_buffer = create_index_buffer(indices.data(), gsl::narrow<uint32>(indices.size()), usage);
    }
    //-------------------------------------------------------------------------
    rex::Mesh::~Mesh()
    {
        m_vertex_buffer->release();
        m_index_buffer->release();
    }

    //-------------------------------------------------------------------------
    void rex::Mesh::set_vertices(std::vector<Vertex>& vertices)
    {
        m_vertex_buffer.reset();
        m_vertex_buffer = create_vertex_buffer(vertices.data(), gsl::narrow<uint32>(vertices.size()), get_usage(), m_buffer_layout);
    }
    //-------------------------------------------------------------------------
    void rex::Mesh::set_indices(std::vector<TriangleIndices>& indices)
    {
        m_index_buffer.reset();
        m_index_buffer = create_index_buffer(indices.data(), gsl::narrow<uint32>(indices.size()), get_usage());
    }

    //-------------------------------------------------------------------------
    const ref_ptr<rex::VertexBuffer>& rex::Mesh::get_vertex_buffer() const
    {
        return m_vertex_buffer;
    }
    //-------------------------------------------------------------------------
    const ref_ptr<rex::IndexBuffer>& rex::Mesh::get_index_buffer() const
    {
        return m_index_buffer;
    }

    //-------------------------------------------------------------------------
    uint32 rex::Mesh::get_index_buffer_count() const
    {
        return static_cast<uint32>(m_index_buffer->get_count());
    }
    //-------------------------------------------------------------------------
    uint32 rex::Mesh::get_vertex_buffer_count() const
    {
        return static_cast<uint32>(m_vertex_buffer->get_count());
    }

    // Static Mesh
    //-------------------------------------------------------------------------
    rex::StaticMesh::StaticMesh()
        : Mesh()
    {
    }
    //-------------------------------------------------------------------------
    rex::StaticMesh::StaticMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices)
        : Mesh(vertices, indices, BufferUsage::STATIC_DRAW)
    {
    }
    //-------------------------------------------------------------------------
    rex::StaticMesh::~StaticMesh()
    {
    }

    //-------------------------------------------------------------------------
    rex::BufferUsage StaticMesh::get_usage() const
    {
        return BufferUsage::STATIC_DRAW;
    }

    // Dynamic Mesh
    //-------------------------------------------------------------------------
    rex::DynamicMesh::DynamicMesh()
        : Mesh()
    {
    }
    //-------------------------------------------------------------------------
    rex::DynamicMesh::DynamicMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices)
        : Mesh(vertices, indices, BufferUsage::DYNAMIC_DRAW)
    {
    }
    //-------------------------------------------------------------------------
    rex::DynamicMesh::~DynamicMesh()
    {
    }

    //-------------------------------------------------------------------------
    rex::BufferUsage DynamicMesh::get_usage() const
    {
        return BufferUsage::DYNAMIC_DRAW;
    }
}
