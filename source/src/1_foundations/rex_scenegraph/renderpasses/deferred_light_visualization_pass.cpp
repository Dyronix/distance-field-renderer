#include "rex_scenegraph_pch.h"

#include "renderpasses/deferred_light_visualization_pass.h"

#include "scene_renderer.h"

#include "renderer/environment.h"
#include "renderer/pipeline.h"
#include "renderer/render_pass.h"
#include "renderer/renderer.h"

#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/uniform_buffer_set.h"

#include "mesh_factory.h"
#include "model.h"

#include "ecs/scene.h"
#include "ecs/scene_camera.h"

#include "renderer/point_light.h"

namespace rex
{
    const int32 MAX_NR_LIGHTS = 32;

    //-------------------------------------------------------------------------
    DeferredLightVisualizationPass::DeferredLightVisualizationPass(const DeferredLightVisualizationPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    DeferredLightVisualizationPass::~DeferredLightVisualizationPass()
    {
        
    }

    //-------------------------------------------------------------------------
    void DeferredLightVisualizationPass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription deferredlight_framebuffer_desc;

            deferredlight_framebuffer_desc.width = vp_width;
            deferredlight_framebuffer_desc.height = vp_height;
            deferredlight_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            deferredlight_framebuffer_desc.depth_attachment = std::move(create_depth_attachment_description(vp_width, vp_height, Texture::Format::DEPTH_COMPONENT_24_INTEGER));
            deferredlight_framebuffer_desc.name = "DeferredLightVisualization";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(deferredlight_framebuffer_desc), FrameBufferDepthAttachmentOption::DEPTH_ONLY);
        }

        RenderPassDescription deferredlight_renderpass_desc;

        deferredlight_renderpass_desc.framebuffer = framebuffer;
        deferredlight_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        deferredlight_renderpass_desc.clear_flags = CLEAR_NONE;
        deferredlight_renderpass_desc.clear_depth = 1.0f;
        deferredlight_renderpass_desc.name = "DeferredLightVisualization";

        PipelineDescription deferredlight_pipeline_desc;

        deferredlight_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        deferredlight_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC3, "a_Normal"}, {DataType::VEC3, "a_Tangent"}, {DataType::VEC3, "a_Binormal"}, {DataType::VEC2, "a_TexCoord"}};
        deferredlight_pipeline_desc.renderpass = Renderer::create_render_pass(deferredlight_renderpass_desc);
        deferredlight_pipeline_desc.name = "DeferredLightVisualization";
        deferredlight_pipeline_desc.depth_test_state = {DepthTestEnabled::YES, DepthTestFunction::LESS, DepthBufferReadOnly::NO};
        deferredlight_pipeline_desc.facecull_state = {FaceCullingEnabled::YES, FaceCullingCullMode::BACK, FaceWinding::COUNTER_CLOCKWISE};

        create_pipeline(deferredlight_pipeline_desc);
        create_material(deferredlight_pipeline_desc.shader, "DeferredLightVisualization");

        m_unit_cube = mesh_factory::get_unit_cube();
    }

    //-------------------------------------------------------------------------
    void DeferredLightVisualizationPass::on_shutdown()
    {
        m_unit_cube.reset();
    }

    //-------------------------------------------------------------------------
    void DeferredLightVisualizationPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
    }

    //-------------------------------------------------------------------------
    void DeferredLightVisualizationPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);

        int32 viewport_width = get_scene_renderer()->get_viewport_width();
        int32 viewport_height = get_scene_renderer()->get_viewport_height();

        RectI src_rect(0, 0, viewport_width, viewport_height);
        RectI dst_rect(0, 0, viewport_width, viewport_height);

        StringID color_name = m_options.color_pass_name;
        StringID depth_name = m_options.depth_pass_name;
        StringID dst_name = m_options.pass_name;

        uint32 color_fb = get_scene_renderer()->get_scene_render_pass(color_name)->get_pipeline()->get_render_pass()->get_frame_buffer()->get_id();
        uint32 depth_fb = get_scene_renderer()->get_scene_render_pass(depth_name)->get_pipeline()->get_render_pass()->get_frame_buffer()->get_id();
        uint32 dst_fb = get_scene_renderer()->get_scene_render_pass(dst_name)->get_pipeline()->get_render_pass()->get_frame_buffer()->get_id();

        Renderer::copy_framebuffer_content(color_fb, src_rect, dst_fb, dst_rect, FrameBufferCopyOption::COLOR, FrameBufferFilterOption::NEAREST);
        Renderer::copy_framebuffer_content(depth_fb, src_rect, dst_fb, dst_rect, FrameBufferCopyOption::DEPTH, FrameBufferFilterOption::NEAREST);
                
        auto& lights = get_scene()->get_light_environment().point_lights;

        for (const auto& pair : lights)
        {
            auto& light = pair.second;

            rex::vec3 light_color = rex::vec3(light.color.red, light.color.green, light.color.blue);
            rex::vec3 light_position = light.position;

            rex::matrix4 transform = rex::matrix4(1.0f);

            transform = rex::translate(transform, light_position);            
            transform = rex::scale(transform, rex::vec3(0.1f,0.1f,0.1f));

            Renderer::submit([material = get_material(), light_color]() mutable 
            {
                material->set(create_sid("u_light_color"), light_color); 
            });

            Renderer::render_model_with_material(get_pipeline(), UniformBufferSet::instance(), m_unit_cube, transform, get_material());
        }

        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void DeferredLightVisualizationPass::end()
    {
        // Nothing to implement
    }
} // namespace rex