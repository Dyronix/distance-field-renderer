#pragma once

#include "memory/internalptr.h"

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/indexbuffer.h"
#include "resources/vertexbuffer.h"
#include "resources/texture.h"
#include "resources/material.h"

#include "minmax.h"
#include "mesh.h"

#include <vector>
#include <memory>

namespace rex
{
    struct Vertex;
    struct TriangleIndices;
    struct Submesh;

    class Texture;
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
        Model(ModelCreationInfo& info);
        ~Model();

        void setMaterial(uint32 index, const ref_ptr<Material>& material);

        const StringID& getName() const;

        uint32 getVertexCount() const;
        uint32 getIndexCount() const;

        const ref_ptr<VertexBuffer>& getVertexBuffer() const;
        const ref_ptr<IndexBuffer>& getIndexBuffer() const;

        const std::vector<Submesh>& getSubmeshes() const;
        const MaterialContainer& getMaterials() const;

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