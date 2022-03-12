#pragma once

#include "memory/ref_counted_object.h"

#include "blob.h"

namespace rex
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

        virtual void flush(IsRenderThread isRenderThread = IsRenderThread::NO) const = 0;

        virtual void set_flag(MaterialFlag flag) = 0;
        virtual void clear_flag(MaterialFlag flag) = 0;
        virtual uint32 get_flags() const = 0;
        virtual bool has_flag(MaterialFlag flag) const = 0;

        virtual void set(const StringID& name, float value) = 0;
        virtual void set(const StringID& name, int32 value) = 0;
        virtual void set(const StringID& name, uint32 value) = 0;
        virtual void set(const StringID& name, bool value) = 0;
        virtual void set(const StringID& name, const rex::vec2& value) = 0;
        virtual void set(const StringID& name, const rex::vec3& value) = 0;
        virtual void set(const StringID& name, const rex::vec4& value) = 0;
        virtual void set(const StringID& name, const rex::vec2i& value) = 0;
        virtual void set(const StringID& name, const rex::vec3i& value) = 0;
        virtual void set(const StringID& name, const rex::vec4i& value) = 0;
        virtual void set(const StringID& name, const rex::matrix3& value) = 0;
        virtual void set(const StringID& name, const rex::matrix4& value) = 0;
        virtual void set_texture2d(const StringID& name, const ref_ptr<Texture>& texture) = 0;
        virtual void set_texture3d(const StringID& name, const ref_ptr<Texture>& texture) = 0;
        virtual void set_texture_cube(const StringID& name, const ref_ptr<TextureCube>& texture) = 0;

        virtual float get_float(const StringID& name) = 0;
        virtual int32 get_int(const StringID& name) = 0;
        virtual uint32 get_u_int(const StringID& name) = 0;
        virtual bool get_bool(const StringID& name) = 0;
        virtual rex::vec2 get_vector2(const StringID& name) = 0;
        virtual rex::vec3 get_vector3(const StringID& name) = 0;
        virtual rex::vec4 get_vector4(const StringID& name) = 0;
        virtual rex::matrix3 get_matrix3(const StringID& name) = 0;
        virtual rex::matrix4 get_matrix4(const StringID& name) = 0;
        virtual const ref_ptr<Texture> get_texture2d(const StringID& name) = 0;
        virtual const ref_ptr<Texture> get_texture3d(const StringID& name) = 0;
        virtual const ref_ptr<TextureCube> get_texture_cube(const StringID& name) = 0;

        virtual ref_ptr<ShaderProgram>& get_shader_program() = 0;
        virtual const ref_ptr<ShaderProgram>& get_shader_program() const = 0;
        virtual const StringID& get_name() const = 0;

        virtual const memory::Blob& get_uniform_buffer_storage() const = 0;
    };
}