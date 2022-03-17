#pragma once

#include "renderer/renderer_api.h"

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

            void set_viewport(const rex::vec2& origin, const rex::vec2& size) override;

            void begin_frame() override;
            void begin_render_pass(ref_ptr<rex::Pipeline> pipeline, bool explicitClear = false) override;
            void end_frame() override;
            void end_render_pass() override;

            void render_quad(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Material> material, const rex::matrix4& transform) override;
            void render_model(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Model> model, const rex::matrix4& transform) override;
            void render_model_with_material(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Model> model, const rex::matrix4& transform, ref_ptr<rex::Material> material) override;

            void copy_framebuffer_content(uint32 fromFrameBufferID, const RectI& fromRect, uint32 toFrameBufferID, const RectI& toRect, const FrameBufferCopyOption& copyOption, const FrameBufferFilterOption& filterOption) override;

            void submit_fullscreen_quad(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Material> material) override;

            ref_ptr<rex::Pipeline> create_pipeline(const PipelineDescription& description) const override;
            ref_ptr<rex::RenderPass> create_render_pass(const RenderPassDescription& description) const override;
        };
    }
}