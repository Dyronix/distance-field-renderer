#include "rex_graphics_pch.h"

#include "model.h"

#include "submesh.h"
#include "vertex.h"
#include "triangle.h"

#include "resources/texture.h"
#include "resources/material.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Model::Model(ModelCreationInfo& info)
        :m_name(info.name)
        ,m_mesh(info.vertices, info.indices)
        ,m_transform(info.transform)
        ,m_inverse_transform(info.inverse_transform)
        ,m_bounding_box(info.bounding_box)
        ,m_submeshes(info.submeshes)
        ,m_diffuse_textures(info.diffuse_textures)
        ,m_normal_textures(info.normal_textures)
        ,m_roughness_textures(info.roughness_textures)
        ,m_materials(std::move(info.materials))
    {}
    //-------------------------------------------------------------------------
    Model::~Model()
    {
        for (ref_ptr<Texture>& texture : m_diffuse_textures)
        {
            texture->release();
        }
        for (ref_ptr<Texture>& texture : m_normal_textures)
        {
            texture->release();
        }
        for (ref_ptr<Texture>& texture : m_roughness_textures)
        {
            texture->release();
        }

        m_materials.clear();
    }

    //-------------------------------------------------------------------------
    void Model::setMaterial(uint32 index, const ref_ptr<Material>& material)
    {
        if (index >= m_materials.size())
        {
            m_materials.push_back(material);
        }
        else
        {
            m_materials[index] = material;
        }
    }

    //-------------------------------------------------------------------------
    const StringID& Model::getName() const
    {
        return m_name;
    }

    //-------------------------------------------------------------------------
    uint32 Model::getVertexCount() const
    {
        return m_mesh.getVertexBufferCount();
    }
    //-------------------------------------------------------------------------
    uint32 Model::getIndexCount() const
    {
        return m_mesh.getIndexBufferCount();
    }

    //-------------------------------------------------------------------------
    const ref_ptr<rex::VertexBuffer>& Model::getVertexBuffer() const
    {
        return m_mesh.getVertexBuffer();
    }
    //-------------------------------------------------------------------------
    const ref_ptr<rex::IndexBuffer>& Model::getIndexBuffer() const
    {
        return m_mesh.getIndexBuffer();
    }

    //-------------------------------------------------------------------------
    const std::vector<rex::Submesh>& Model::getSubmeshes() const
    {
        return m_submeshes;
    }

    //-------------------------------------------------------------------------
    const MaterialContainer& Model::getMaterials() const
    {
        return m_materials;
    }
}