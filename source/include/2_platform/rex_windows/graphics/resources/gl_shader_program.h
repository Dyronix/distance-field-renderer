#pragma once

#include "memory/internal_ptr.h"

#include "resources/shader_program.h"

namespace rex
{
    namespace opengl
    {
        class ShaderProgram : public rex::ShaderProgram
        {
        public:
            ShaderProgram(ShaderProgramCreationInfo&& info);
            ~ShaderProgram();

            bool is_bound() const override;
            bool is_linked() const override;

            const StringID& get_name() const override;

            uint32 get_queue() const override;
            uint32 get_shader_program_id() const override;

            const ShaderUniform& get_uniform(const StringID& name) const override;
            const ShaderUniformLocation& get_uniform_location(const StringID& name) const override;
            const ShaderUniforms get_uniforms() const override;
            const int32 get_uniform_count() const override;

            const ShaderAttribute& get_attribute(const StringID& name) const override;
            const ShaderAttributes get_attributes() const override;
            const int32 get_attribute_count() const override;

            const ShaderResource& get_resource(const StringID& name) const override;
            const ShaderResourceLocation& get_resource_location(const StringID& name) const override;
            const ShaderResources get_resources() const override;
            const int32 get_resource_count() const override;

            const ShaderUniformBlock& get_uniform_block(const StringID& name) const override;
            const ShaderUniformBlocks get_uniform_blocks() const override;
            const int32 get_uniform_block_count() const override;

            void release() override;
            void bind(IsRenderThread rt = IsRenderThread::NO) const override;
            void unbind(IsRenderThread rt = IsRenderThread::NO) const override;

            void set_uniform(const StringID& name, const rex::matrix3& mat, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const rex::matrix4& mat, IsRenderThread rt = IsRenderThread::NO) const override;

            void set_uniform(const StringID& name, const float value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const rex::vec2& value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const rex::vec3& value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const rex::vec4& value, IsRenderThread rt = IsRenderThread::NO) const override;

            void set_uniform(const StringID& name, const rex::vec2i& value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const rex::vec3i& value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const rex::vec4i& value, IsRenderThread rt = IsRenderThread::NO) const override;

            void set_uniform(const StringID& name, const float* values, uint32 size, IsRenderThread rt = IsRenderThread::NO) const override;

            void set_uniform(const StringID& name, const int32 value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const uint32 value, IsRenderThread rt = IsRenderThread::NO) const override;
            void set_uniform(const StringID& name, const bool value, IsRenderThread rt = IsRenderThread::NO) const override;

        private:
            struct Internal;
            rex::internal_scope_ptr<Internal> m_internal;
        };
    }
}