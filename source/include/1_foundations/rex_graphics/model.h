#pragma once

#include "resources/texture.h"
#include "resources/material.h"

#include "min_max.h"
#include "mesh.h"

namespace rex
{
    struct Vertex;
    struct TriangleIndices;
    struct Submesh;

    class VertexBuffer;
    class IndexBuffer;

    using MaterialContainer = std::vector<ref_ptr<Material>>;

    struct ModelCreationInfo
    {
        StringID name;

        std::vector<Vertex> vertices;
        std::vector<TriangleIndices> indices;

        rex::matrix4 transform;
        rex::matrix4 inverse_transform;

        AABB bounding_box;

        std::vector<Submesh> submeshes;

        std::vector<ref_ptr<Texture>> diffuse_textures;
        std::vector<ref_ptr<Texture>> normal_textures;
        std::vector<ref_ptr<Texture>> roughness_textures;

        MaterialContainer materials;
    };

    class Model : public RefCountedObject
    {
    public:
        Model(ModelCreationInfo&& info) noexcept;
        Model(StaticMesh&& staticMesh) noexcept;
        ~Model();

        void set_material(uint32 index, const ref_ptr<Material>& material);

        const StringID& get_name() const;

        uint32 get_vertex_count() const;
        uint32 get_index_count() const;

        const ref_ptr<VertexBuffer>& get_vertex_buffer() const;
        const ref_ptr<IndexBuffer>& get_index_buffer() const;

        const std::vector<Submesh>& get_submeshes() const;
        const MaterialContainer& get_materials() const;

    private:
        StringID m_name;

        StaticMesh m_mesh;

        rex::matrix4 m_transform;
        rex::matrix4 m_inverse_transform;

        AABB m_bounding_box;

        std::vector<Submesh> m_submeshes;

        std::vector<ref_ptr<Texture>> m_diffuse_textures;
        std::vector<ref_ptr<Texture>> m_normal_textures;
        std::vector<ref_ptr<Texture>> m_roughness_textures;

        MaterialContainer m_materials;
    };
}