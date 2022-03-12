#pragma once

#include "renderer/pipeline.h"
#include "renderer/render_pass.h"

namespace rex
{
    class UniformBufferSet;
    class Material;
    class Model;

    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        virtual void initialize() = 0;
        virtual void shutdown() = 0;

        virtual void set_viewport(const rex::vec2& origin, const rex::vec2& size) = 0;

        virtual void begin_frame() = 0;
        virtual void begin_render_pass(ref_ptr<Pipeline> pipeline, bool explicitClear = false) = 0;
        virtual void end_frame() = 0;
        virtual void end_render_pass() = 0;

        virtual void render_quad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material, const rex::matrix4& transform) = 0;
        virtual void render_model(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform) = 0;
        virtual void render_model_with_material(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform, ref_ptr<Material> material) = 0;

        virtual void submit_fullscreen_quad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material) = 0;

        virtual ref_ptr<Pipeline> create_pipeline(const PipelineDescription& description) const = 0;
        virtual ref_ptr<RenderPass> create_render_pass(const RenderPassDescription& description) const = 0;

    protected:
        RendererAPI() = default;
    };
}