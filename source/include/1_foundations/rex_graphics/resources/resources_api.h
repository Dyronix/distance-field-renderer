#pragma once

#include "resources/texture3ddescription.h"
#include "resources/texture2ddescription.h"
#include "resources/texturecubedescription.h"
#include "resources/texture.h"

#include "resources/shaderprogram.h"
#include "resources/framebuffer.h"
#include "resources/uniformbuffer.h"
#include "resources/vertexbuffer.h"
#include "resources/indexbuffer.h"
#include "resources/material.h"
#include "resources/shaderuniformblock.h"

#include "usage.h"

namespace sbt
{
    struct Vertex;
    struct TriangleIndices;

    class ResourcesAPI
    {
    public:
        ResourcesAPI() = default;
        virtual ~ResourcesAPI() = default;

        virtual ref_ptr<sbt::Texture> createTexture2D(const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture2D(uint32 width, uint32 height, const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture2D(Texture::Data&& textureData, uint32 width, uint32 height,
                                                      const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture2D(void* textureData, size_t textureDataSize, uint32 width, uint32 height,
                                                      const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture2D(Texture2DDescription&& desc) const = 0;

        virtual ref_ptr<sbt::Texture> createTexture3D(const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture3D(uint32 width, uint32 height, uint32 depth, const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture3D(Texture::Data&& textureData, uint32 width, uint32 height, uint32 depth,
                                                      const StringID& name = EName::SID_None,
                                                      const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::Texture> createTexture3D(Texture3DDescription&& desc) const = 0;

        virtual ref_ptr<sbt::TextureCube> createTextureCube(const StringID& name = EName::SID_None,
                                                            const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::TextureCube> createTextureCube(uint32 width, uint32 height, const StringID& name = EName::SID_None,
                                                            const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::TextureCube> createTextureCube(Texture::Data&& textureData, uint32 width, uint32 height,
                                                            const StringID& name = EName::SID_None,
                                                            const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::TextureCube> createTextureCube(void* textureData, size_t textureDataSize, uint32 width, uint32 height,
                                                            const StringID& name = EName::SID_None,
                                                            const Texture::Usage& textureUsage = Texture::Usage::UNSPECIFIED) const = 0;
        virtual ref_ptr<sbt::TextureCube> createTextureCube(TextureCubeDescription&& desc) const = 0;

        virtual ref_ptr<sbt::ShaderProgram> createShaderProgram(ShaderProgramCreationInfo&& creationInfo) const = 0;
        virtual ref_ptr<sbt::FrameBuffer> createFrameBuffer(FrameBufferDescription&& description,
                                                            DepthAttachmentOption depthAttachmentOption = DepthAttachmentOption::NONE) = 0;
        virtual StringID createUniformBuffer(const ShaderUniformBlock& block) const = 0;

        virtual ref_ptr<sbt::VertexBuffer> createVertexBuffer(void* vertices, uint32 size, uint32 count) const = 0;
        virtual ref_ptr<sbt::VertexBuffer> createVertexBuffer(void* vertices, uint32 size, uint32 count, Usage usage) const = 0;
        virtual ref_ptr<sbt::IndexBuffer> createIndexBuffer(TriangleIndices* indices, uint32 count) const = 0;
        virtual ref_ptr<sbt::IndexBuffer> createIndexBuffer(TriangleIndices* indices, uint32 count, Usage usage) const = 0;

        virtual ref_ptr<sbt::Material> createMaterial(ref_ptr<ShaderProgram> shader) const = 0;
        virtual ref_ptr<sbt::Material> createMaterial(ref_ptr<ShaderProgram> shader, const StringID& name) const = 0;
    };
}