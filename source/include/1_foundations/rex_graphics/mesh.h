#pragma once

#include "usage.h"

#include "memory/refcountedobject.h"
#include "memory/refptr.h"

#include "resources/bufferlayout.h"

namespace rex
{
    struct Vertex;
    struct TriangleIndices;

    class VertexBuffer;
    class IndexBuffer;

    class IMesh
    {
    public:        
        virtual ~IMesh() = default;

        virtual Usage getUsage() const = 0;

        virtual const ref_ptr<rex::VertexBuffer>& getVertexBuffer() const = 0;
        virtual const ref_ptr<rex::IndexBuffer>& getIndexBuffer() const = 0;

        virtual uint32 getIndexBufferCount() const = 0;
        virtual uint32 getVertexBufferCount() const = 0;

    protected:
        IMesh() = default;

        static BufferLayout makeBufferLayout();

        static ref_ptr<rex::VertexBuffer> createVertexBuffer(Vertex* vertices, uint32 count, Usage usage, const BufferLayout& layout);
        static ref_ptr<rex::IndexBuffer> createIndexBuffer(TriangleIndices* indices, uint32 count, Usage usage);
    };

    class Mesh : public IMesh
    {
    public:
        Mesh();
        Mesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices, Usage usage);
        ~Mesh() override;

        void setVertices(std::vector<Vertex>& vertices);
        void setIndices(std::vector<TriangleIndices>& indices);

        const ref_ptr<rex::VertexBuffer>& getVertexBuffer() const override;
        const ref_ptr<rex::IndexBuffer>& getIndexBuffer() const override;

        virtual uint32 getIndexBufferCount() const override;
        virtual uint32 getVertexBufferCount() const override;

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

        Usage getUsage() const override;
    };

    class DynamicMesh : public Mesh
    {
    public:
        DynamicMesh();
        DynamicMesh(std::vector<Vertex>& vertices, std::vector<TriangleIndices>& indices);
        ~DynamicMesh() override;

        Usage getUsage() const override;
    };
}