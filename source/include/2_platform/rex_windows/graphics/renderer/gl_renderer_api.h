#pragma once

#include "renderer/rendererapi.h"

namespace rex
{
    class Pipeline;
    class RenderPass;
    class Material;
    class Model;
    class UniformBufferSet;

    namespace opengl
    {
        class RendererAPI : public rex::RendererAPI
        {
        public:
            RendererAPI();
            ~RendererAPI() override;

            void initialize() override;
            void shutdown() override;

            void setViewport(const rex::vec2& origin, const rex::vec2& size) override;

            void beginFrame() override;
            void beginRenderPass(ref_ptr<rex::Pipeline> pipeline, bool explicitClear = false) override;
            void endFrame() override;
            void endRenderPass() override;

            void renderQuad(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Material> material, const rex::matrix4& transform) override;
            void renderModel(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Model> model, const rex::matrix4& transform) override;
            void renderModelWithMaterial(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Model> model, const rex::matrix4& transform, ref_ptr<rex::Material> material) override;

            void submitFullscreenQuad(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Material> material) override;

            ref_ptr<rex::Pipeline> createPipeline(const PipelineDescription& description) const override;
            ref_ptr<rex::RenderPass> createRenderPass(const RenderPassDescription& description) const override;
        };
    }
}