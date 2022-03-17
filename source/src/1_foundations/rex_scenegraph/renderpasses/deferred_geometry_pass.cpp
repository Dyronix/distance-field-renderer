#include "rex_scenegraph_pch.h"

#include "renderpasses/deferred_geometry_pass.h"

#include "scene_renderer.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/environment.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/texture_library.h"
#include "resources/uniform_buffer_set.h"
#include "resources/material.h"
#include "resources/texture_2d.h"
#include "model.h"

#include "ecs/scene.h"
#include "ecs/scene_camera.h"

namespace rex
{
    const StringID GEOMETRY_PIPELINE_NAME = "Geometry"_sid;

    //-------------------------------------------------------------------------
    DeferredGeometryPass::DeferredGeometryPass(const DeferredGeometryPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    DeferredGeometryPass::~DeferredGeometryPass()
    {
    }

    //-------------------------------------------------------------------------
    void DeferredGeometryPass::on_initialize(const ref_ptr<SceneRenderer>& /*renderer*/)
    {
        create_geometry_pipeline();
    }

    //-------------------------------------------------------------------------
    void DeferredGeometryPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DeferredGeometryPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
    }
    //-------------------------------------------------------------------------
    void DeferredGeometryPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);

        for (auto& dc : get_scene_renderer()->get_draw_commands())
        {
            if (dc.override_material)
            {
                Renderer::render_model_with_material(get_pipeline(), UniformBufferSet::instance(), dc.model, dc.transform, dc.override_material);
            }
            else
            {
                Renderer::render_model(get_pipeline(), UniformBufferSet::instance(), dc.model, dc.transform);
            }
        }

        Renderer::end_render_pass();
    }
    //-------------------------------------------------------------------------
    void DeferredGeometryPass::end()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DeferredGeometryPass::create_geometry_pipeline()
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if (m_create_framebuffer)
        {
            uint32 vp_width = get_scene_renderer()->get_viewport_width();
            uint32 vp_height = get_scene_renderer()->get_viewport_height();

            FrameBufferDescription geometry_framebuffer_desc;

            geometry_framebuffer_desc.width = vp_width;
            geometry_framebuffer_desc.height = vp_height;
            geometry_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            geometry_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            geometry_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            geometry_framebuffer_desc.depth_attachment = std::move(create_depth_attachment_description(vp_width, vp_height, Texture::Format::DEPTH_COMPONENT_24_INTEGER));
            geometry_framebuffer_desc.name = "Geometry";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(geometry_framebuffer_desc), FrameBufferDepthAttachmentOption::DEPTH_ONLY);
        }

        RenderPassDescription geometry_renderpass_desc;

        geometry_renderpass_desc.framebuffer = framebuffer;
        geometry_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        geometry_renderpass_desc.clear_depth = 1.0f;
        geometry_renderpass_desc.clear_flags = CLEAR_COLOR_AND_DEPTH;
        geometry_renderpass_desc.name = "Geometry";

        PipelineDescription geometry_pipeline_desc;

        geometry_pipeline_desc.shader = shader_library::get(m_options.geometry_shader_name);
        geometry_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC3, "a_Normal"}, {DataType::VEC3, "a_Tangent"}, {DataType::VEC3, "a_Binormal"}, {DataType::VEC2, "a_TexCoord"}};
        geometry_pipeline_desc.renderpass = Renderer::create_render_pass(geometry_renderpass_desc);
        geometry_pipeline_desc.depth_test_state = {DepthTestEnabled::YES, DepthTestFunction::LESS, DepthBufferReadOnly::NO};
        geometry_pipeline_desc.name = GEOMETRY_PIPELINE_NAME;

        if (m_options.backface_culling)
        {
            geometry_pipeline_desc.facecull_state = {FaceCullingEnabled::YES, FaceCullingCullMode::BACK, FaceWinding::COUNTER_CLOCKWISE};
        }
        else
        {
            geometry_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        }

        create_pipeline(geometry_pipeline_desc);
    }
}