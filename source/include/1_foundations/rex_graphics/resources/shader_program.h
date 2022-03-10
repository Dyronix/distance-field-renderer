#pragma once

#include "yesnoenum.h"
#include "types.h"
#include "memoryblob.h"

#include "string/stringid.h"

#include "apitype.h"

#include "resources/apiresource.h"
#include "resources/shaderuniform.h"
#include "resources/shaderattribute.h"
#include "resources/shaderresource.h"
#include "resources/shaderuniformblock.h"

#include <string>

namespace sbt
{
    class ShaderUniformLocation
    {
    public:
        static ShaderUniformLocation EMPTY;

        //-------------------------------------------------------------------------
        ShaderUniformLocation()
            : m_location(-1)
        {
        }
        //-------------------------------------------------------------------------
        ShaderUniformLocation(int32 loc)
            : m_location(loc)
        {
        }

        //-------------------------------------------------------------------------
        operator bool() const
        {
            return m_location != -1;
        }
        //-------------------------------------------------------------------------
        operator int32() const
        {
            return m_location;
        }

        //-------------------------------------------------------------------------
        int32 get() const
        {
            return m_location;
        }

    private:
        int32 m_location;
    };
    class ShaderResourceLocation
    {
    public:
        static ShaderResourceLocation EMPTY;

        //-------------------------------------------------------------------------
        ShaderResourceLocation()
            : m_location(-1)
        {
        }
        //-------------------------------------------------------------------------
        ShaderResourceLocation(int32 loc)
            : m_location(loc)
        {
        }

        //-------------------------------------------------------------------------
        operator bool() const
        {
            return m_location != -1;
        }
        //-------------------------------------------------------------------------
        operator int32() const
        {
            return m_location;
        }

        //-------------------------------------------------------------------------
        int32 get() const
        {
            return m_location;
        }

    private:
        int32 m_location;
    };

    struct ShaderProgramCreationInfo
    {
        StringID tag;
        StringID queue;

        API::Type api;

        memory::Blob vertexShaderCode;
        memory::Blob fragmentShaderCode;

        ShaderAttributes attributes;
        ShaderUniforms uniforms;
        ShaderResources resources;
        ShaderUniformBlocks blocks;
    };

    class ShaderProgram : public Resource
    {
    public:
        ShaderProgram() = default;
        virtual ~ShaderProgram() = default;

        virtual bool isBound() const = 0;
        virtual bool isLinked() const = 0;

        virtual const StringID& getName() const = 0;

        virtual uint32 getQueue() const = 0;
        virtual uint32 getShaderProgramID() const = 0;
        virtual API::Type getRendererAPI() const = 0;

        virtual const ShaderUniform& getUniform(const StringID& name) const = 0;
        virtual const ShaderUniformLocation& getUniformLocation(const StringID& name) const = 0;
        virtual const ShaderUniforms getUniforms() const = 0;
        virtual const int32 getUniformCount() const = 0;

        virtual const ShaderAttribute& getAttribute(const StringID& name) const = 0;
        virtual const ShaderAttributes getAttributes() const = 0;
        virtual const int32 getAttributeCount() const = 0;

        virtual const ShaderResource& getResource(const StringID& name) const = 0;
        virtual const ShaderResourceLocation& getResourceLocation(const StringID& name) const = 0;
        virtual const ShaderResources getResources() const = 0;
        virtual const int32 getResourceCount() const = 0;

        virtual const ShaderUniformBlock& getUniformBlock(const StringID& name) const = 0;
        virtual const ShaderUniformBlocks getUniformBlocks() const = 0;
        virtual const int32 getUniformBlockCount() const = 0;

        virtual void bind(IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void unbind(IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void setUniform(const StringID& name, const sbt::matrix3& mat, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const sbt::matrix4& mat, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void setUniform(const StringID& name, const float value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const sbt::vec2& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const sbt::vec3& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const sbt::vec4& value, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void setUniform(const StringID& name, const sbt::vec2i& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const sbt::vec3i& value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const sbt::vec4i& value, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void setUniform(const StringID& name, const float* values, uint32 size, IsRenderThread rt = IsRenderThread::NO) const = 0;

        virtual void setUniform(const StringID& name, const int32 value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const uint32 value, IsRenderThread rt = IsRenderThread::NO) const = 0;
        virtual void setUniform(const StringID& name, const bool value, IsRenderThread rt = IsRenderThread::NO) const = 0;
    };
}