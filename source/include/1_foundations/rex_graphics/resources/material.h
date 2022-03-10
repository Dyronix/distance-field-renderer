#pragma once

#include "defines.h"
#include "types.h"

#include "memory/refcountedobject.h"
#include "memory/refptr.h"

#include "string/stringid.h"
#include "string/stringids.h"

#include <memory>

namespace sbt
{
    class ShaderProgram;
    class Texture;
    class TextureCube;

    enum class MaterialFlag
    {
        None = BIT(0),
        DepthTest = BIT(1),
        Blend = BIT(2),
        TwoSided = BIT(3)
    };

    class Material : public RefCountedObject
    {
    public:
        Material() = default;
        virtual ~Material() = default;

        virtual void                            flush(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;

        virtual void                            setFlag(MaterialFlag flag) = 0;
        virtual void                            clearFlag(MaterialFlag flag) = 0;
        virtual uint32                          getFlags() const = 0;
        virtual bool                            hasFlag(MaterialFlag flag) const = 0;

        virtual void                            set(const StringID& name, float value) = 0;
        virtual void                            set(const StringID& name, int32 value) = 0;
        virtual void                            set(const StringID& name, uint32 value) = 0;
        virtual void                            set(const StringID& name, bool value) = 0;
        virtual void                            set(const StringID& name, const sbt::vec2& value) = 0;
        virtual void                            set(const StringID& name, const sbt::vec3& value) = 0;
        virtual void                            set(const StringID& name, const sbt::vec4& value) = 0;
        virtual void                            set(const StringID& name, const sbt::vec2i& value) = 0;
        virtual void                            set(const StringID& name, const sbt::vec3i& value) = 0;
        virtual void                            set(const StringID& name, const sbt::vec4i& value) = 0;
        virtual void                            set(const StringID& name, const sbt::matrix3& value) = 0;
        virtual void                            set(const StringID& name, const sbt::matrix4& value) = 0;
        virtual void                            setTexture2D(const StringID& name, const ref_ptr<Texture>& texture) = 0;
        virtual void                            setTexture3D(const StringID& name, const ref_ptr<Texture>& texture) = 0;
        virtual void                            setTextureCube(const StringID& name, const ref_ptr<TextureCube>& texture) = 0;

        virtual float                           getFloat(const StringID& name) = 0;
        virtual int32                           getInt(const StringID& name) = 0;
        virtual uint32                          getUInt(const StringID& name) = 0;
        virtual bool                            getBool(const StringID& name) = 0;
        virtual sbt::vec2                       getVector2(const StringID& name) = 0;
        virtual sbt::vec3                       getVector3(const StringID& name) = 0;
        virtual sbt::vec4                       getVector4(const StringID& name) = 0;
        virtual sbt::matrix3                       getMatrix3(const StringID& name) = 0;
        virtual sbt::matrix4                       getMatrix4(const StringID& name) = 0;
        virtual const ref_ptr<Texture>          getTexture2D(const StringID& name) = 0;
        virtual const ref_ptr<Texture>          getTexture3D(const StringID& name) = 0;
        virtual const ref_ptr<TextureCube>      getTextureCube(const StringID& name) = 0;

        virtual ref_ptr<ShaderProgram>&         getShaderProgram() = 0;
        virtual const ref_ptr<ShaderProgram>&   getShaderProgram() const = 0;
        virtual const StringID&                 getName() const = 0;

        virtual const memory::Blob&             getUniformBufferStorage() const = 0;
    };
}