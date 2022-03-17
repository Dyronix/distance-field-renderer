#include "rex_scenegraph_pch.h"

#include "renderpasses/forward_geometry_pass.h"

#include "scene_renderer.h"
#include "mesh_factory.h"
#include "model.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/environment.h"

#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/uniform_buffer_set.h"
#include "resources/material.h"

#include "ecs/scene.h"
#include "ecs/scene_camera.h"
#include "ecs/components/model_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/material_component.h"

namespace rex
{
    const StringID GEOMETRY_PIPELINE_NAME = "Geometry"_sid;

    const StringID GRID_PIPELINE_NAME = "Grid"_sid;
    const StringID GRID_MATERIAL_NAME = "Grid"_sid;

    const StringID SKYBOX_PIPELINE_NAME = "Skybox"_sid;
    const StringID SKYBOX_MATERIAL_NAME = "Skybox"_sid;

    //-------------------------------------------------------------------------
    ForwardGeometryPass::ForwardGeometryPass(const ForwardGeometryPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_active_environment(ESID::SID_None)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    ForwardGeometryPass::~ForwardGeometryPass()
    {
    }

    //-------------------------------------------------------------------------
    void ForwardGeometryPass::on_initialize(const ref_ptr<SceneRenderer>& /*renderer*/)
    {
        create_geometry_pipeline();
        create_sky_box_pipeline();
        create_grid_pipeline();
    }

    //-------------------------------------------------------------------------
    void ForwardGeometryPass::on_shutdown()
    {
        m_sky_model.reset();
    }

    //-------------------------------------------------------------------------
    void ForwardGeometryPass::enable_grid()
    {
        m_options.show_grid = true;
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::disable_grid()
    {
        m_options.show_grid = false;
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::toggle_grid()
    {
        m_options.show_grid = !m_options.show_grid;
    }

    //-------------------------------------------------------------------------
    void ForwardGeometryPass::enable_skybox()
    {
        m_options.show_environment = true;
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::disable_skybox()
    {
        m_options.show_environment = false;
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::toggle_skybox()
    {
        m_options.show_environment = !m_options.show_environment;
    }

    //-------------------------------------------------------------------------
    void ForwardGeometryPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        UNUSED_PARAM(camera);
        UNUSED_PARAM(cameraTransform);
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(GEOMETRY_PIPELINE_NAME), Renderer::ExplicitClear::YES);

        for (auto& dc : get_scene_renderer()->get_draw_commands())
        {
            if (dc.override_material)
            {
                Renderer::render_model_with_material(get_pipeline(GEOMETRY_PIPELINE_NAME), UniformBufferSet::instance(), dc.model, dc.transform, dc.override_material);
            }
            else
            {
                Renderer::render_model(get_pipeline(GEOMETRY_PIPELINE_NAME), UniformBufferSet::instance(), dc.model, dc.transform);
            }
        }

        if (m_options.show_grid)
        {
            const rex::matrix4 transform = rex::rotate(rex::matrix4(1.0f), rex::deg2rad(-90.0f), rex::vec3(1.0f, 0.0f, 0.0f)) * rex::scale(rex::matrix4(1.0f), rex::vec3(8.0f));
            Renderer::render_quad(get_pipeline(GRID_PIPELINE_NAME), UniformBufferSet::instance(), get_material(GRID_MATERIAL_NAME), transform);
        }

        if (m_options.show_environment)
        {
            const Environment& env = get_scene()->get_environment();
            if (env.skybox_texture_cube->get_name() != m_active_environment)
            {
                get_material(SKYBOX_MATERIAL_NAME)->set_texture_cube("u_Texture", get_scene()->get_environment().skybox_texture_cube);
            }

            Renderer::render_model_with_material(get_pipeline(SKYBOX_PIPELINE_NAME), UniformBufferSet::instance(), m_sky_model, rex::matrix4(1.0), get_material(SKYBOX_MATERIAL_NAME));
        }

        Renderer::end_render_pass();
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::end()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void ForwardGeometryPass::create_geometry_pipeline()
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
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::create_sky_box_pipeline()
    {
        PipelineDescription skybox_pipeline_desc;

        skybox_pipeline_desc.shader = shader_library::get(m_options.skybox_shader_name);
        skybox_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC3, "a_Normal"}, {DataType::VEC3, "a_Tangent"}, {DataType::VEC3, "a_Binormal"}, {DataType::VEC2, "a_TexCoord"}};
        skybox_pipeline_desc.renderpass = get_pipeline()->get_render_pass();
        skybox_pipeline_desc.depth_test_state = {DepthTestEnabled::YES, DepthTestFunction::LESS_OR_EQUAL, DepthBufferReadOnly::NO};
        skybox_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        skybox_pipeline_desc.rasterizer_state = {FillMode::FILL};
        skybox_pipeline_desc.name = SKYBOX_PIPELINE_NAME;

        create_pipeline(skybox_pipeline_desc);

        auto skybox_material = create_material(skybox_pipeline_desc.shader, SKYBOX_MATERIAL_NAME);
        skybox_material->set_texture_cube("u_Texture", get_scene()->get_environment().skybox_texture_cube);

        m_sky_model = mesh_factory::get_unit_cube();

        // Book keeping
        m_active_environment = get_scene()->get_environment().skybox_texture_cube->get_name();
    }
    //-------------------------------------------------------------------------
    void ForwardGeometryPass::create_grid_pipeline()
    {
        PipelineDescription grid_pipeline_desc;

        grid_pipeline_desc.shader = shader_library::get(m_options.grid_shader_name);
        grid_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC3, "a_Normal"}, {DataType::VEC3, "a_Tangent"}, {DataType::VEC3, "a_Binormal"}, {DataType::VEC2, "a_TexCoord"}};
        grid_pipeline_desc.renderpass = get_pipeline()->get_render_pass();
        grid_pipeline_desc.depth_test_state = {DepthTestEnabled::YES, DepthTestFunction::LESS, DepthBufferReadOnly::NO};
        grid_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        grid_pipeline_desc.name = GRID_PIPELINE_NAME;

        create_pipeline(grid_pipeline_desc);

        auto grid_material = create_material(grid_pipeline_desc.shader, GRID_MATERIAL_NAME);
        grid_material->set("u_GridScale", m_options.grid_scale);
        grid_material->set("u_GridSize", m_options.grid_size);
    }
}