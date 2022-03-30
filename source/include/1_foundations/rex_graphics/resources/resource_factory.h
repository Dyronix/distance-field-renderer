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

    class ResourceFactory
    {
    public:
        static void initialize(std::unique_ptr<ResourcesAPI> api);
        static void shutdown();

        static ref_ptr<rex::Texture> create_texture2d(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture2d(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture2d(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture2d(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture2d(Texture2DDescription&& desc);

        static ref_ptr<rex::Texture> create_texture3d(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture3d(uint32 width, uint32 height, uint32 depth, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture3d(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::Texture> create_texture3d(Texture3DDescription&& desc);

        static ref_ptr<rex::TextureCube> create_texture_cube(const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::TextureCube> create_texture_cube(uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::TextureCube> create_texture_cube(Texture::Data&& textureData, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::TextureCube> create_texture_cube(void* textureData, size_t textureDataSize, uint32 width, uint32 height, const StringID& name = ESID::SID_None, const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED);
        static ref_ptr<rex::TextureCube> create_texture_cube(TextureCubeDescription&& desc);

        static ref_ptr<rex::ShaderProgram> create_shader_program(ShaderProgramCreationInfo&& creationInfo);
        static ref_ptr<rex::FrameBuffer> create_frame_buffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption);
        static StringID create_uniform_buffer(const ShaderUniformBlock& block);

        static ref_ptr<rex::VertexBuffer> create_vertex_buffer(void* vertices, uint32 size, uint32 count);
        static ref_ptr<rex::VertexBuffer> create_vertex_buffer(void* vertices, uint32 size, uint32 count, BufferUsage usage);
        static ref_ptr<rex::IndexBuffer> create_index_buffer(TriangleIndices* indices, uint32 count);
        static ref_ptr<rex::IndexBuffer> create_index_buffer(TriangleIndices* indices, uint32 count, BufferUsage usage);
        static ref_ptr<rex::IndexBuffer> create_index_buffer(int32* indices, uint32 count);
        static ref_ptr<rex::IndexBuffer> create_index_buffer(int32* indices, uint32 count, BufferUsage usage);

        static ref_ptr<rex::Material> create_material(ref_ptr<ShaderProgram> shader);
        static ref_ptr<rex::Material> create_material(ref_ptr<ShaderProgram> shader, const StringID& name);
    };
}