#pragma once

#include "api_type.h"

namespace rex
{
    struct PipelineDescription;
    class Pipeline;

    struct RenderPassDescription;
    class RenderPass;

    class UniformBufferSet;
    class Material;
    class Model;

    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        virtual void initialize() = 0;
        virtual void shutdown() = 0;

        virtual void setViewport(const rex::vec2& origin, const rex::vec2& size) = 0;

        virtual void beginFrame() = 0;
        virtual void beginRenderPass(ref_ptr<Pipeline> pipeline, bool explicitClear = false) = 0;
        virtual void endFrame() = 0;
        virtual void endRenderPass() = 0;

        virtual void renderQuad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material, const rex::matrix4& transform) = 0;
        virtual void renderModel(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform) = 0;
        virtual void renderModelWithMaterial(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform, ref_ptr<Material> material) = 0;

        virtual void submitFullscreenQuad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material) = 0;

        virtual ref_ptr<Pipeline> createPipeline(const PipelineDescription& description) const = 0;
        virtual ref_ptr<RenderPass> createRenderPass(const RenderPassDescription& description) const = 0;

    protected:
        RendererAPI() = default;

    private:
        static API::Type s_renderer_api;
    };
}