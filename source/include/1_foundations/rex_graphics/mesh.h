#pragma once

#include "resources/buffer_layout.h"

namespace rex
{
    enum class BufferUsage;

    struct Vertex;
    struct TriangleIndices;

    class VertexBuffer;
    class IndexBuffer;

    class IMesh
    {
    public:
        virtual ~IMesh() = default;

        virtual BufferUsage get_usage() const = 0;

        virtual const ref_ptr<rex::VertexBuffer>& get_vertex_buffer() const = 0;
        virtual const ref_ptr<rex::IndexBuffer>& get_index_buffer() const = 0;

        virtual uint32 get_index_buffer_count() const = 0;
        virtual uint32 get_vertex_buffer_count() const = 0;

    protected:
        IMesh() = default;

        static BufferLayout make_buffer_layout();

        static ref_ptr<rex::VertexBuffer> create_vertex_buffer(Vertex* vertices, uint32 count, BufferUsage usage, const BufferLayout& layout);
        static ref_ptr<rex::IndexBuffer> create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage);
    };

    class Mesh : public IMesh
    {
    public:
        Mesh();
        Mesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices, BufferUsage usage);
        ~Mesh() override;

        void set_vertices(std::vector<Vertex>& vertices);
        void set_indices(std::vector<TriangleIndices>& indices);

        const ref_ptr<rex::VertexBuffer>& get_vertex_buffer() const override;
        const ref_ptr<rex::IndexBuffer>& get_index_buffer() const override;

        virtual uint32 get_index_buffer_count() const override;
        virtual uint32 get_vertex_buffer_count() const override;

    private:
        ref_ptr<VertexBuffer> m_vertex_buffer;
        ref_ptr<IndexBuffer> m_index_buffer;

        BufferLayout m_buffer_layout;
    };

    class StaticMesh : public Mesh
    {
    public:
        StaticMesh();
        StaticMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices);
        ~StaticMesh() override;

        BufferUsage get_usage() const override;
    };

    class DynamicMesh : public Mesh
    {
    public:
        DynamicMesh();
        DynamicMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices);
        ~DynamicMesh() override;

        BufferUsage get_usage() const override;
    };
}