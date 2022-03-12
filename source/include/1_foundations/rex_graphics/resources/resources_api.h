#pragma once

#include "resources/texture.h"
#include "resources/texture_cube.h"

namespace rex
{
    class ResourcesAPI;
    class ShaderUniformBlock;
    class Material;
    class ShaderProgram;
    class VertexBuffer;
    class IndexBuffer;
    class FrameBuffer;

    struct Vertex;
    struct TriangleIndices;
    struct Texture2DDescription;
    struct Texture3DDescription;
    struct TextureCubeDescription;
    struct ShaderProgramCreationInfo;
    struct FrameBufferDescription;

    enum class BufferUsage;
    enum class FrameBufferDepthAttachmentOption;    

    class ResourcesAPI
    {
    public:
        ResourcesAPI() = default;
        virtual ~ResourcesAPI() = default;

        virtual ref_ptr<rex::Texture> create_texture2d(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture2d(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture2d(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture2d(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture2d(Texture2DDescription&& desc) const = 0;

        virtual ref_ptr<rex::Texture> create_texture3d(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture3d(uint32 width, uint32 height, uint32 depth, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture3d(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::Texture> create_texture3d(Texture3DDescription&& desc) const = 0;

        virtual ref_ptr<rex::TextureCube> create_texture_cube(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::TextureCube> create_texture_cube(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::TextureCube> create_texture_cube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::TextureCube> create_texture_cube(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<rex::TextureCube> create_texture_cube(TextureCubeDescription&& desc) const = 0;

        virtual ref_ptr<rex::ShaderProgram> create_shader_program(ShaderProgramCreationInfo&& creationInfo) const = 0;
        virtual ref_ptr<rex::FrameBuffer> create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption) = 0;
        virtual StringID create_uniform_buffer(const ShaderUniformBlock& block) const = 0;

        virtual ref_ptr<rex::VertexBuffer> create_vertex_buffer(void* vertices, uint32 size, uint32 count) const = 0;
        virtual ref_ptr<rex::VertexBuffer> create_vertex_buffer(void* vertices, uint32 size, uint32 count, BufferUsage usage) const = 0;
        virtual ref_ptr<rex::IndexBuffer> create_index_buffer(TriangleIndices* indices, uint32 count) const = 0;
        virtual ref_ptr<rex::IndexBuffer> create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage) const = 0;

        virtual ref_ptr<rex::Material> create_material(ref_ptr<ShaderProgram> shader) const = 0;
        virtual ref_ptr<rex::Material> create_material(ref_ptr<ShaderProgram> shader, const StringID& name) const = 0;
    };
}