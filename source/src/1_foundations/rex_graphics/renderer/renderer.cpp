#include "rex_graphics_pch.h"

#include "renderer/renderer.h"
#include "renderer/renderer_api.h"

#include "resources/resource_factory.h"
#include "resources/texture.h"
#include "resources/texture_cube.h"
#include "resources/material.h"
#include "resources/uniform_buffer_set.h"

#include "model.h"
#include "color.h"

namespace rex
{
    std::unique_ptr<RendererAPI>            g_renderer_api = nullptr;
    std::unique_ptr<RendererCommandQueue>   g_renderer_command_queue = nullptr;

    std::unique_ptr<SolidColorTexture>      g_white_texture = nullptr;
    std::unique_ptr<SolidColorTexture>      g_black_texture = nullptr;
    std::unique_ptr<SolidColorTextureCube>  g_white_texture_cube = nullptr;
    std::unique_ptr<SolidColorTextureCube>  g_black_texture_cube = nullptr;

    bool                                    g_wait_and_render = false;

    StringID                                g_active_render_pass_name;

    //-------------------------------------------------------------------------
    void Renderer::initialize(std::unique_ptr<RendererAPI> api)
    {
        g_renderer_command_queue = std::make_unique<RendererCommandQueue>();

        g_renderer_api = std::move(api);
        g_renderer_api->initialize();

        g_white_texture = std::make_unique<SolidColorTexture>(rgba{ 0xffffffff });
        g_black_texture = std::make_unique<SolidColorTexture>(rgba{ 0xff000000 });

        constexpr uint8 NUM_FACES_CUBE = 6;

        rgba white_texture_cube_data[NUM_FACES_CUBE] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
        rgba black_texture_cube_data[NUM_FACES_CUBE] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };

        g_white_texture_cube = std::make_unique<SolidColorTextureCube>(white_texture_cube_data, NUM_FACES_CUBE);
        g_black_texture_cube = std::make_unique<SolidColorTextureCube>(black_texture_cube_data, NUM_FACES_CUBE);
    }

    //-------------------------------------------------------------------------
    void Renderer::shutdown()
    {
        g_white_texture_cube->release();
        g_black_texture_cube->release();

        g_black_texture->release();
        g_white_texture->release();

        wait_and_render();

        g_renderer_api->shutdown();
        g_renderer_api.reset();

        g_renderer_command_queue.reset();
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::Texture>& Renderer::get_white_texture()
    {
        return g_white_texture->get_texture();
    }
    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::Texture>& Renderer::get_black_texture()
    {
        return g_black_texture->get_texture();
    }
    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::TextureCube>& Renderer::get_white_texture_cube()
    {
        return g_white_texture_cube->get_texture();
    }
    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::TextureCube>& Renderer::get_black_texture_cube()
    {
        return g_black_texture_cube->get_texture();
    }

    //-------------------------------------------------------------------------
    void Renderer::set_viewport(const rex::vec2& origin, const rex::vec2& size)
    {
        g_renderer_api->set_viewport(origin, size);
    }

    //-------------------------------------------------------------------------
    void Renderer::begin_frame()
    {
        g_renderer_api->begin_frame();
    }
    //-------------------------------------------------------------------------
    void Renderer::begin_render_pass(ref_ptr<Pipeline> pipeline, ExplicitClear explicitClear)
    {
        g_active_render_pass_name = pipeline->get_render_pass()->get_debug_name();

        RENDERER_INFO("Renderer::beginRenderPass: {0}", g_active_render_pass_name.to_string());
        g_renderer_api->begin_render_pass(pipeline, explicitClear);
    }

    //-------------------------------------------------------------------------
    void Renderer::end_frame()
    {
        g_renderer_api->end_frame();
    }
    //-------------------------------------------------------------------------
    void Renderer::end_render_pass()
    {
        g_renderer_api->end_render_pass();
        RENDERER_INFO("Renderer::endRenderPass: {0}", g_active_render_pass_name.to_string());

        g_active_render_pass_name = EName::SID_None;
    }

    //-------------------------------------------------------------------------
    void Renderer::wait_and_render()
    {
        RENDERER_INFO("----------------------------------------------------------------------------------");
        RENDERER_INFO("Renderer::waitAndRender - start");

        if (g_renderer_command_queue)
        {
            g_wait_and_render = true;

            g_renderer_command_queue->execute();
            g_renderer_command_queue->clear();

            g_wait_and_render = false;
        }

        RENDERER_INFO("Renderer::waitAndRender - end");
        RENDERER_INFO("----------------------------------------------------------------------------------");
    }

    //-------------------------------------------------------------------------
    void Renderer::render_quad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material, const rex::matrix4& transform)
    {
        g_renderer_api->render_quad(pipeline, uniformBufferSet, material, transform);
    }
    //-------------------------------------------------------------------------
    void Renderer::renderModel(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform)
    {
        g_renderer_api->render_model(pipeline, uniformBufferSet, model, transform);
    }
    //-------------------------------------------------------------------------
    void Renderer::render_model_with_material(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform, ref_ptr<Material> material)
    {
        g_renderer_api->render_model_with_material(pipeline, uniformBufferSet, model, transform, material);
    }

    //-------------------------------------------------------------------------
    void Renderer::submit_fullscreen_quad(ref_ptr<Pipeline> pipeline, UniformBufferSet* uniformBufferSet, ref_ptr<Material> material)
    {
        g_renderer_api->submit_fullscreen_quad(pipeline, uniformBufferSet, material);
    }

    //-------------------------------------------------------------------------
    ref_ptr<rex::Pipeline> Renderer::create_pipeline(const PipelineDescription& description)
    {
        return g_renderer_api->create_pipeline(description);
    }
    //-------------------------------------------------------------------------
    ref_ptr<rex::RenderPass> Renderer::create_render_pass(const RenderPassDescription& description)
    {
        return g_renderer_api->create_render_pass(description);
    }

    //-------------------------------------------------------------------------
    rex::RendererCommandQueue* Renderer::get_render_command_queue()
    {
        return g_renderer_command_queue.get();
    }
}