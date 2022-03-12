#pragma once

#include "memory/internal_ptr.h"

#include "resources/material.h"

namespace rex
{
    namespace opengl
    {
        class Material : public rex::Material
        {
        public:
            Material(ref_ptr<rex::ShaderProgram> shader, const StringID& name = ESID::SID_None);
            ~Material() override;

            void flush(IsRenderThread isRenderThread = IsRenderThread::NO) const override;

            void set_flag(MaterialFlag flag) override;
            void clear_flag(MaterialFlag flag) override;
            uint32 get_flags() const override;
            bool has_flag(MaterialFlag flag) const override;

            void set(const StringID& name, float value) override;
            void set(const StringID& name, int32 value) override;
            void set(const StringID& name, uint32 value) override;
            void set(const StringID& name, bool value) override;
            void set(const StringID& name, const rex::vec2& value) override;
            void set(const StringID& name, const rex::vec3& value) override;
            void set(const StringID& name, const rex::vec4& value) override;
            void set(const StringID& name, const rex::vec2i& value) override;
            void set(const StringID& name, const rex::vec3i& value) override;
            void set(const StringID& name, const rex::vec4i& value) override;
            void set(const StringID& name, const rex::matrix3& value) override;
            void set(const StringID& name, const rex::matrix4& value) override;
            void set_texture2d(const StringID& name, const ref_ptr<rex::Texture>& texture) override;
            void set_texture3d(const StringID& name, const ref_ptr<Texture>& texture) override;
            void set_texture_cube(const StringID& name, const ref_ptr<rex::TextureCube>& texture) override;

            float get_float(const StringID& name) override;
            int32 get_int(const StringID& name) override;
            uint32 get_u_int(const StringID& name) override;
            bool get_bool(const StringID& name) override;
            rex::vec2 get_vector2(const StringID& name) override;
            rex::vec3 get_vector3(const StringID& name) override;
            rex::vec4 get_vector4(const StringID& name) override;
            rex::matrix3 get_matrix3(const StringID& name) override;
            rex::matrix4 get_matrix4(const StringID& name) override;
            const ref_ptr<rex::Texture> get_texture2d(const StringID& name) override;
            const ref_ptr<rex::Texture> get_texture3d(const StringID& name) override;
            const ref_ptr<rex::TextureCube> get_texture_cube(const StringID& name) override;

            ref_ptr<rex::ShaderProgram>& get_shader_program() override;
            const ref_ptr<rex::ShaderProgram>& get_shader_program() const override;
            const StringID& get_name() const override;

            const memory::Blob& get_uniform_buffer_storage() const override;

        private:
            struct Internal;
            rex::internal_scope_ptr<Internal> m_internal;
        };
    }
}