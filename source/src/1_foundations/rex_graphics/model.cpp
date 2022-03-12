#include "rex_graphics_pch.h"

#include "model.h"

#include "submesh.h"
#include "vertex.h"
#include "triangle.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Model::Model(ModelCreationInfo& info)
        : m_name(info.name)
        , m_mesh(info.vertices, info.indices)
        , m_transform(info.transform)
        , m_inverse_transform(info.inverse_transform)
        , m_bounding_box(info.bounding_box)
        , m_submeshes(info.submeshes)
        , m_diffuse_textures(info.diffuse_textures)
        , m_normal_textures(info.normal_textures)
        , m_roughness_textures(info.roughness_textures)
        , m_materials(std::move(info.materials))
    {
    }
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
    void Model::set_material(uint32 index, const ref_ptr<Material>& material)
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
    const StringID& Model::get_name() const
    {
        return m_name;
    }

    //-------------------------------------------------------------------------
    uint32 Model::get_vertex_count() const
    {
        return m_mesh.get_vertex_buffer_count();
    }
    //-------------------------------------------------------------------------
    uint32 Model::get_index_count() const
    {
        return m_mesh.get_index_buffer_count();
    }

    //-------------------------------------------------------------------------
    const ref_ptr<rex::VertexBuffer>& Model::get_vertex_buffer() const
    {
        return m_mesh.get_vertex_buffer();
    }
    //-------------------------------------------------------------------------
    const ref_ptr<rex::IndexBuffer>& Model::get_index_buffer() const
    {
        return m_mesh.get_index_buffer();
    }

    //-------------------------------------------------------------------------
    const std::vector<rex::Submesh>& Model::get_submeshes() const
    {
        return m_submeshes;
    }

    //-------------------------------------------------------------------------
    const MaterialContainer& Model::get_materials() const
    {
        return m_materials;
    }
}