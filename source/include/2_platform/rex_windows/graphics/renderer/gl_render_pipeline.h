#pragma once

#include "renderer/pipeline.h"

namespace rex
{
    class ShaderPipeline;
    class RenderPass;

    namespace opengl
    {
        class Pipeline : public rex::Pipeline
        {
        public:
            Pipeline(const PipelineDescription& pipelineDesc);
            ~Pipeline() override;

            ref_ptr<rex::ShaderProgram>&        get_shader_program() override;
            const ref_ptr<rex::ShaderProgram>&  get_shader_program() const override;
            ref_ptr<rex::RenderPass>&           get_render_pass() override;
            const ref_ptr<rex::RenderPass>&     get_render_pass() const override;

            const BufferLayout&                 get_buffer_layout() const override;

            const PrimitiveType&                get_primitive_type() const override;
            const FaceCullState&                get_face_cull_state() const override;
            const DepthTestState&               get_depth_test_state() const override;
            const RasterizerState&              get_rasterizer_state() const override;

            const StringID&                     get_debug_name() const override;

            void                                invalidate() override;
            void                                release() override;

            // TEMP: remove this when render command buffers are a thing
            void                                bind() const override;
            void                                unbind() const override;

        private:
            PipelineDescription m_description;

            uint32 m_vertex_array_id;
        };
    }
}