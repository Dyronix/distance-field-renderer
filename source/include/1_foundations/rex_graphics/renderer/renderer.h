#pragma once

#include "yes_no_enum.h"

#include "renderer/renderer_command_queue.h"
#include "renderer/pipeline.h"
#include "renderer/render_pass.h"

#include "rect.h"

#define ENABLE_RENDERER_LOGGING 0
#if ENABLE_RENDERER_LOGGING
#define RENDERER_INFO R_INFO
#define RENDERER_WARN R_WARN
#define RENDERER_ERROR R_ERROR
#else
#define RENDERER_INFO(...) UNUSED_PARAM(__VA_ARGS__)
#define RENDERER_WARN(...) UNUSED_PARAM(__VA_ARGS__)
#define RENDERER_ERROR(...) UNUSED_PARAM(__VA_ARGS__)
#endif

namespace rex
{
    class Model;
    class Material;
    class RendererAPI;
    class UniformBufferSet;
    class Texture;
    class TextureCube;

    enum class FrameBufferCopyOption;
    enum class FrameBufferFilterOption;

    class Renderer
    {
    public:
        using ExplicitClear = YesNoEnum;

        Renderer() = delete;
        ~Renderer() = delete;

        static void initialize(std::unique_ptr<RendererAPI> api);
        static void shutdown();

        static const int32 get_max_combined_texture_image_units();
        static const int32 get_max_texture_size();

        static const ref_ptr<Texture>& get_white_texture();
        static const ref_ptr<Texture>& get_black_texture();
        static const ref_ptr<TextureCube>& get_white_texture_cube();
        static const ref_ptr<TextureCube>& get_black_texture_cube();

        static void set_viewport(const rex::vec2& origin, const rex::vec2& size);

        static void begin_frame();
        static void begin_render_pass(ref_ptr<Pipeline> pipeline, ExplicitClear explicitClear = ExplicitClear::NO);
        static void end_frame();
        static void end_render_pass();

        static void wait_and_render();

        static void render_quad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material, const rex::matrix4& transform);
        static void render_model(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform);
        static void render_model_with_material(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform, ref_ptr<Material> material);

        static void copy_framebuffer_content(uint32 fromFrameBufferID, const RectI& fromRect, uint32 toFrameBufferID, const RectI& toRect, const FrameBufferCopyOption& copyOption, const FrameBufferFilterOption& filterOption);
        static void read_framebuffer_content(const RectI& rect, const Texture::Format& textureFormat, const Texel::Format& texelFormat);

        static void submit_fullscreen_quad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material);
        template <typename TFunction>
        static void submit(TFunction&& fn);

        static ref_ptr<Pipeline> create_pipeline(const PipelineDescription& description);
        static ref_ptr<RenderPass> create_render_pass(const RenderPassDescription& description);

    private:
        static RendererCommandQueue* get_render_command_queue();
    };

    template <typename TFunction>
    void rex::Renderer::submit(TFunction&& fn)
    {
        R_ASSERT_X(get_render_command_queue()->get_state() == RenderCommandQueueState::ACCEPT, "Cannot push new commands while executing or clearing render commands");

        get_render_command_queue()->push(std::move(fn));
    }
}