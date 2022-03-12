#include "rex_windows_pch.h"

#include "graphics/resources/gl_resources_api.h"

#include "graphics/resources/gl_texture_2d.h"
#include "graphics/resources/gl_texture_3d.h"
#include "graphics/resources/gl_texture_cube.h"

#include "graphics/resources/gl_shader_program.h"
#include "graphics/resources/gl_frame_buffer.h"
#include "graphics/resources/gl_uniform_buffer.h"

#include "graphics/resources/gl_vertex_buffer.h"
#include "graphics/resources/gl_index_buffer.h"

#include "graphics/resources/gl_material.h"

#include "mesh.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        ResourcesAPI::ResourcesAPI() = default;
        //-------------------------------------------------------------------------
        ResourcesAPI::~ResourcesAPI() = default;

        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture2d(const StringID& name /* = ESID::SID_None */, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture2D>(name, textureUsage));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture2d(uint32 width, uint32 height, const StringID& name /* = ESID::SID_None */, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture2D>(width, height, name, textureUsage));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture2d(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name /* = ESID::SID_None */, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture2D>(std::move(textureData), width, height, name, textureUsage));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture2d(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name /* = ESID::SID_None */,
                                                             const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return create_texture2d(memory::make_blob(reinterpret_cast<memory::byte*>(textureData), textureDataSize), width, height, name, textureUsage);
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture2d(Texture2DDescription&& desc) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture2D>(std::move(desc)));
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture3d(const StringID& name, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture3D>(name, textureUsage));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture3d(uint32 width, uint32 height, uint32 depth, const StringID& name, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture3D>(width, height, depth, name, textureUsage));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture3d(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth, const StringID& name, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture3D>(std::move(textureData), width, height, depth, name, textureUsage));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Texture> ResourcesAPI::create_texture3d(Texture3DDescription&& desc) const
        {
            return ref_ptr<rex::Texture>(make_ref<opengl::Texture3D>(std::move(desc)));
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::TextureCube> ResourcesAPI::create_texture_cube(const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::TextureCube>(make_ref<opengl::TextureCube>(name, textureUsage));
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::TextureCube> ResourcesAPI::create_texture_cube(uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::TextureCube>(make_ref<opengl::TextureCube>(width, height, name, textureUsage));
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::TextureCube> ResourcesAPI::create_texture_cube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/, const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return ref_ptr<rex::TextureCube>(make_ref<opengl::TextureCube>(std::move(textureData), width, height, name, textureUsage));
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::TextureCube> ResourcesAPI::create_texture_cube(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name /*= ESID::SID_None*/,
                                                                         const Texture::Usage& textureUsage /*= Texture::Usage::UNSPECIFIED*/) const
        {
            return create_texture_cube(memory::make_blob(reinterpret_cast<memory::byte*>(textureData), textureDataSize), width, height, name, textureUsage);
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::TextureCube> ResourcesAPI::create_texture_cube(TextureCubeDescription&& desc) const
        {
            return ref_ptr<rex::TextureCube>(make_ref<opengl::TextureCube>(std::move(desc)));
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::ShaderProgram> ResourcesAPI::create_shader_program(ShaderProgramCreationInfo&& creationInfo) const
        {
            return ref_ptr<rex::ShaderProgram>(make_ref<opengl::ShaderProgram>(std::move(creationInfo)));
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::FrameBuffer> ResourcesAPI::create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption)
        {
            return opengl::FrameBuffer::create(std::move(description), depthAttachmentOption);
        }

        //-------------------------------------------------------------------------
        StringID ResourcesAPI::create_uniform_buffer(const ShaderUniformBlock& block) const
        {
            return opengl::UniformBuffer::create(block);
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::VertexBuffer> ResourcesAPI::create_vertex_buffer(void* vertices, uint32 size, uint32 count) const
        {
            return ref_ptr<rex::VertexBuffer>(make_ref<opengl::VertexBuffer>(vertices, size, count, BufferUsage::STATIC_DRAW));
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::VertexBuffer> ResourcesAPI::create_vertex_buffer(void* vertices, uint32 size, uint32 count, BufferUsage usage) const
        {
            return ref_ptr<rex::VertexBuffer>(make_ref<opengl::VertexBuffer>(vertices, size, count, usage));
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::IndexBuffer> ResourcesAPI::create_index_buffer(TriangleIndices* indices, uint32 count) const
        {
            return create_index_buffer(indices, count, BufferUsage::STATIC_DRAW);
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::IndexBuffer> ResourcesAPI::create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage) const
        {
            return ref_ptr<rex::IndexBuffer>(make_ref<opengl::IndexBuffer>(indices, count, usage));
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::Material> ResourcesAPI::create_material(ref_ptr<rex::ShaderProgram> shader) const
        {
            return create_material(shader, ESID::SID_None);
        }
        //-------------------------------------------------------------------------
        ref_ptr<rex::Material> ResourcesAPI::create_material(ref_ptr<rex::ShaderProgram> shader, const StringID& name) const
        {
            return ref_ptr<rex::Material>(make_ref<opengl::Material>(shader, name));
        }
    }
}