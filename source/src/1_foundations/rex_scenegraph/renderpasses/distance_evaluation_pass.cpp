#include "rex_scenegraph_pch.h"

#include "renderpasses/distance_evaluation_pass.h"

#include "scene_renderer.h"

#include "renderer/renderer.h"
#include "renderer/render_pass.h"
#include "renderer/pipeline.h"
#include "renderer/environment.h"

#include "resources/material.h"
#include "resources/resource_factory.h"
#include "resources/shader_library.h"
#include "resources/uniform_buffer_set.h"

#include "ecs/scene_camera.h"
#include "ecs/scene.h"

#include "renderer/point_light.h"

namespace rex
{
    //-------------------------------------------------------------------------
    rex::matrix4 generate_frustrum_matrix(const rex::AspectRatio::Ratio& aspectRatio, const rex::FieldOfView& fov)
    {
        rex::matrix4 frustum_corners = rex::identity<rex::matrix4>();

        float fov_v_half = fov.get_vertical().to_rad() * 0.5f;
        float tan_fov = std::tan(fov_v_half);

        rex::vec3 to_right = rex::world_right<float>() * tan_fov * aspectRatio.get();
        rex::vec3 to_top = rex::world_up<float>() * tan_fov;

        rex::vec3 top_left = (-rex::world_forward<float>() - to_right + to_top);
        rex::vec3 top_right = (-rex::world_forward<float>() + to_right + to_top);
        rex::vec3 bottom_right = (-rex::world_forward<float>() + to_right - to_top);
        rex::vec3 bottom_left = (-rex::world_forward<float>() - to_right - to_top);

        frustum_corners = rex::column(frustum_corners, 0, rex::vec4(top_left, 0.0f));
        frustum_corners = rex::column(frustum_corners, 1, rex::vec4(top_right, 0.0f));
        frustum_corners = rex::column(frustum_corners, 2, rex::vec4(bottom_right, 0.0f));
        frustum_corners = rex::column(frustum_corners, 3, rex::vec4(bottom_left, 0.0f));

        return frustum_corners;
    }

    //-------------------------------------------------------------------------
    DistanceEvaluationPass::DistanceEvaluationPass(const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer)
        : SceneRenderPass(options.pass_name)
        , m_options(options)
        , m_create_framebuffer(create_frame_buffer)
    {
    }

    //-------------------------------------------------------------------------
    DistanceEvaluationPass::~DistanceEvaluationPass()
    {
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::on_initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        ref_ptr<FrameBuffer> framebuffer = nullptr;

        if(m_create_framebuffer)
        {
            uint32 vp_width = renderer->get_viewport_width();
            uint32 vp_height = renderer->get_viewport_height();

            FrameBufferDescription sdf_framebuffer_desc;

            sdf_framebuffer_desc.width = vp_width;
            sdf_framebuffer_desc.height = vp_height;
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.color_attachments.push_back(std::move(create_color_attachment_description(vp_width, vp_height, Texture::Format::RGBA_32_FLOAT)));
            sdf_framebuffer_desc.depth_attachment = std::move(create_depth_attachment_description(vp_width, vp_height, Texture::Format::DEPTH_COMPONENT_24_INTEGER));
            sdf_framebuffer_desc.name = "SDF";

            framebuffer = ResourceFactory::create_frame_buffer(std::move(sdf_framebuffer_desc), FrameBufferDepthAttachmentOption::NONE);
        }

        RenderPassDescription sdf_renderpass_desc;

        sdf_renderpass_desc.framebuffer = framebuffer;
        sdf_renderpass_desc.clear_color = {0.15f, 0.15f, 0.15f, 1.0f};
        sdf_renderpass_desc.clear_depth = 1.0f;
        sdf_renderpass_desc.clear_flags = CLEAR_COLOR_AND_DEPTH;
        sdf_renderpass_desc.name = "SDF";

        PipelineDescription sdf_pipeline_desc;

        sdf_pipeline_desc.shader = shader_library::get(m_options.shader_name);
        sdf_pipeline_desc.layout = {{DataType::VEC3, "a_Position"}, {DataType::VEC2, "a_TexCoord"}};
        sdf_pipeline_desc.renderpass = Renderer::create_render_pass(sdf_renderpass_desc);
        sdf_pipeline_desc.depth_test_state = {DepthTestEnabled::YES};
        sdf_pipeline_desc.facecull_state = {FaceCullingEnabled::NO};
        sdf_pipeline_desc.name = "SDF";

        create_pipeline(sdf_pipeline_desc);

        auto material = create_material(sdf_pipeline_desc.shader, "SDF");

        // Setup SDF environment
        // Sphere Tracer
        material->set("u_max_iterations", m_options.sphere_tracer_options.max_iterations);
        material->set("u_max_march_distance", m_options.sphere_tracer_options.max_march_distance);
        material->set("u_min_surface_distance", m_options.sphere_tracer_options.min_surface_distance);
        // Scene
        material->set("u_scene_size", m_options.sdf_scene_options.scene_size);
        material->set("u_scene_center", m_options.sdf_scene_options.scene_center);

        R_ASSERT_X(m_options.sdf_scene_options.scene_data != nullptr, "No scene data was given to the \"Distance Evaluation Pass\"");

        material->set_texture3d("u_scene_data", m_options.sdf_scene_options.scene_data);
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::on_shutdown()
    {
        // Nothing to implement
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::begin(const ecs::SceneCamera& camera, const Transform& cameraTransform)
    {
        rex::matrix4 view = rex::look_at(cameraTransform.get_position(), cameraTransform.get_position() + cameraTransform.get_forward(), rex::world_up<float>());
        rex::matrix4 frustrum = generate_frustrum_matrix(camera.get_aspect_ratio(), camera.get_perspective_field_of_view());

        auto material = get_material();

        material->set("u_camera_world", rex::inverse(view));
        material->set("u_camera_frustrum", frustrum);
        material->set("u_camera_position", cameraTransform.get_position());
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::render()
    {
        Renderer::begin_render_pass(get_pipeline(), Renderer::ExplicitClear::YES);
        Renderer::submit_fullscreen_quad(get_pipeline(), UniformBufferSet::instance(), get_material());
        Renderer::end_render_pass();
    }

    //-------------------------------------------------------------------------
    void DistanceEvaluationPass::end()
    {
        // Nothing to implement
    }
}