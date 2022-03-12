#pragma once

#include "blob.h"

#include "resources/uniform_buffer.h"
#include "resources/shader_uniform_block.h"

namespace rex
{
    namespace opengl
    {
        class UniformBuffer : public rex::UniformBuffer
        {
        public:
            static StringID create(const ShaderUniformBlock& block);

            UniformBuffer(const ShaderUniformBlock& block);
            ~UniformBuffer() override;

            void release() override;
            void set_data(const void* data, uint32 size, uint32 offset = 0, IsRenderThread rt = IsRenderThread::NO) override;

            StringID get_name() const override;

            uint32 get_binding() const override;
            uint32 get_size() const override;

            const ShaderUniforms& get_uniforms() const override;

        private:
            uint32 m_buffer_id;
            ShaderUniformBlock m_uniform_buffer_desc;

            memory::Blob m_local_storage;
        };
    }
}