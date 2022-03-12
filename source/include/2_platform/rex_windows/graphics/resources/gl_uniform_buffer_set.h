#pragma once

#include "resources/uniform_buffer_set.h"

namespace rex
{
    namespace opengl
    {
        class UniformBufferSet : public rex::UniformBufferSet
        {
        public:
            UniformBufferSet(uint32 maxFrames = 3u);
            ~UniformBufferSet() override;

        protected:
            ref_ptr<rex::UniformBuffer> create_uniform_buffer(const ShaderUniformBlock& block) const override;
        };
    }
}