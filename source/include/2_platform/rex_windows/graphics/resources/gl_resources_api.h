#pragma once

#include "resources/resources_api.h"

namespace rex
{
    namespace opengl
    {       
        class ResourcesAPI : public rex::ResourcesAPI
        {
        public:
            ResourcesAPI();
            ~ResourcesAPI() override;

            ref_ptr<rex::Texture>       create_texture2d(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture2d(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture2d(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture2d(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture2d(Texture2DDescription&& desc) const override;

            ref_ptr<rex::Texture>       create_texture3d(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture3d(uint32 width, uint32 height, uint32 depth, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture3d(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::Texture>       create_texture3d(Texture3DDescription&& desc) const override;

            ref_ptr<rex::TextureCube>   create_texture_cube(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::TextureCube>   create_texture_cube(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::TextureCube>   create_texture_cube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::TextureCube>   create_texture_cube(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const override;
            ref_ptr<rex::TextureCube>   create_texture_cube(TextureCubeDescription&& desc) const override;

            ref_ptr<rex::ShaderProgram> create_shader_program(ShaderProgramCreationInfo&& creationInfo) const override;
            ref_ptr<rex::FrameBuffer> create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption) override;
            StringID                    create_uniform_buffer(const ShaderUniformBlock& block) const override;

            ref_ptr<rex::VertexBuffer>  create_vertex_buffer(void* vertices, uint32 size, uint32 count) const override;
            ref_ptr<rex::VertexBuffer>  create_vertex_buffer(void* vertices, uint32 size, uint32 count, BufferUsage usage) const override;
            ref_ptr<rex::IndexBuffer>   create_index_buffer(TriangleIndices* indices, uint32 count) const override;
            ref_ptr<rex::IndexBuffer>   create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage) const override;

            ref_ptr<rex::Material>      create_material(ref_ptr<rex::ShaderProgram> shader) const override;
            ref_ptr<rex::Material>      create_material(ref_ptr<rex::ShaderProgram> shader, const StringID& name) const override;
        };
    }
}