#include "rex_graphics_pch.h"

#include "resources/texture_2d_description.h"
#include "resources/texture_3d_description.h"
#include "resources/texture_cube_description.h"
#include "resources/shader_uniform_block.h"
#include "resources/resource_factory.h"
#include "resources/resources_api.h"
#include "resources/material.h"
#include "resources/shader_program.h"
#include "resources/frame_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/index_buffer.h"

#include "buffer_usage.h"

namespace rex
{
    std::unique_ptr<ResourcesAPI> g_resources_api = nullptr;

    //-------------------------------------------------------------------------
    void ResourceFactory::initialize(std::unique_ptr<ResourcesAPI> api)
    {
        R_PROFILE_FUNCTION();

        g_resources_api = std::move(api);
    }
    //-------------------------------------------------------------------------
    void ResourceFactory::shutdown()
    {
        R_PROFILE_FUNCTION();

        g_resources_api.reset();
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture2d(const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture2d(name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture2d(uint32 width, uint32 height, const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture2d(width, height, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture2d(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture2d(std::move(textureData), width, height, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture2d(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture2d(textureData, textureDataSize, width, height, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture2d(Texture2DDescription&& desc)
    {
        return g_resources_api->create_texture2d(std::move(desc));
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture3d(const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture3d(name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture3d(uint32 width, uint32 height, uint32 depth, const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture3d(width, height, depth, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture3d(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth, const StringID& name, const Texture::Usage& textureUsage)
    {
        return g_resources_api->create_texture3d(std::move(textureData), width, height, depth, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Texture> ResourceFactory::create_texture3d(Texture3DDescription&& desc)
    {
        return g_resources_api->create_texture3d(std::move(desc));
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::TextureCube> ResourceFactory::create_texture_cube(const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
    {
        return g_resources_api->create_texture_cube(name, textureUsage);
    }
    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::TextureCube> ResourceFactory::create_texture_cube(uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
    {
        return g_resources_api->create_texture_cube(width, height, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::TextureCube> ResourceFactory::create_texture_cube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
    {
        return g_resources_api->create_texture_cube(std::move(textureData), width, height, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::TextureCube> ResourceFactory::create_texture_cube(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/,
                                                                        const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/)
    {
        return g_resources_api->create_texture_cube(textureData, textureDataSize, width, height, name, textureUsage);
    }
    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::TextureCube> ResourceFactory::create_texture_cube(TextureCubeDescription&& desc)
    {
        return g_resources_api->create_texture_cube(std::move(desc));
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::ShaderProgram> ResourceFactory::create_shader_program(ShaderProgramCreationInfo&& creationInfo)
    {
        return g_resources_api->create_shader_program(std::move(creationInfo));
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::FrameBuffer> ResourceFactory::create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption)
    {
        return g_resources_api->create_frame_buffer(std::move(description), depthAttachmentOption);
    }

    //-------------------------------------------------------------------------
    StringID ResourceFactory::create_uniform_buffer(const ShaderUniformBlock& block)
    {
        return g_resources_api->create_uniform_buffer(block);
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::VertexBuffer> rex::ResourceFactory::create_vertex_buffer(void* vertices, uint32 size, uint32 count)
    {
        return g_resources_api->create_vertex_buffer(vertices, size, count);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::VertexBuffer> rex::ResourceFactory::create_vertex_buffer(void* vertices, uint32 size, uint32 count, BufferUsage usage)
    {
        return g_resources_api->create_vertex_buffer(vertices, size, count, usage);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::IndexBuffer> ResourceFactory::create_index_buffer(TriangleIndices* indices, uint32 count)
    {
        return g_resources_api->create_index_buffer(indices, count);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::IndexBuffer> ResourceFactory::create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage)
    {
        return g_resources_api->create_index_buffer(indices, count, usage);
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::IndexBuffer> ResourceFactory::create_index_buffer(int32* indices, uint32 count)
    {
        return g_resources_api->create_index_buffer(indices, count);
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::IndexBuffer> ResourceFactory::create_index_buffer(int32* indices, uint32 count, BufferUsage usage)
    {
        return g_resources_api->create_index_buffer(indices, count, usage);
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::Material> ResourceFactory::create_material(ref_ptr<ShaderProgram> shader)
    {
        return g_resources_api->create_material(shader);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::Material> ResourceFactory::create_material(ref_ptr<ShaderProgram> shader, const StringID& name)
    {
        return g_resources_api->create_material(shader, name);
    }
}