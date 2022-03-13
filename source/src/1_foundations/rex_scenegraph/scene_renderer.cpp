#include "rex_scenegraph_pch.h"

#include "perspective_camera.h"
#include "model.h"

#include "ecs/scene.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/model_component.h"
#include "ecs/components/material_component.h"
#include "ecs/components/camera_component.h"

#include "scene_renderer.h"

#include "renderer/renderer.h"

#include "resources/uniform_buffer.h"
#include "resources/uniform_buffer_set.h"
#include "resources/material.h"
#include "resources/texture_resource_manager.h"

namespace rex
{
    //-------------------------------------------------------------------------
    ClearPassOptions make_clear_pass_options()
    {
        ClearPassOptions options;

        options.pass_name = "ClearPass"_sid;
        options.shader_name = "blit"_sid;
        options.clear_color = Color(0.15f, 0.15f, 0.15f, 1.0f);

        return options;
    }

    const StringID CAMERA_UNIFORM_BUFFER_NAME = "Camera"_sid;
    const StringID POINTLIGHTS_UNIFORM_BUFFER_NAME = "PointLightData"_sid;
    const StringID DIRECTIONALLIGHTS_UNIFORM_BUFFER_NAME = "DirectionalLightData"_sid;

    //-------------------------------------------------------------------------
    SceneRenderer::SceneRenderer(ref_ptr<ecs::Scene> scene, SceneRenderPasses&& renderPasses)
        : m_previous_scenerenderpass(nullptr)
        , m_current_scenerenderpass(nullptr)
        , m_renderpasses(std::move(renderPasses))
        , m_draw_list()
        , m_active_scene(scene)
        , m_pipeline_setup(false)
        , m_is_active(false)
        , m_clear_pass(make_clear_pass_options(), CreateFrameBuffer::NO)
    {
        for (const auto& pass : m_renderpasses)
        {
            R_TRACE("Scene Render Passes: {0}", pass->get_name().to_string());
        }

        ref_ptr<SceneRenderer> instance(this);

        m_clear_pass.initialize(instance);

        for (const std::unique_ptr<SceneRenderPass>& renderpass : m_renderpasses)
        {
            renderpass->initialize(instance);
        }

        Renderer::submit([instance]() mutable
                         {
                             instance->m_pipeline_setup = true;
                         });
    }
    //-------------------------------------------------------------------------
    SceneRenderer::~SceneRenderer()
    {
        destroy();
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::destroy()
    {
        m_clear_pass.shutdown();

        for (const std::unique_ptr<SceneRenderPass>& renderpass : m_renderpasses)
        {
            renderpass->shutdown();
        }

        m_renderpasses.clear();
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::set_scene(const ref_ptr<ecs::Scene>& scene)
    {
        if (m_active_scene != scene)
        {
            m_active_scene = scene;
        }
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::set_viewport_width(int32 width)
    {
        if (!m_active_scene)
        {
            return;
        }

        if (get_scene()->get_viewport_width() != width)
        {
            get_scene()->set_viewport_width(width);
            m_needs_resize = true;
        }
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::set_viewport_height(int32 height)
    {
        if (!m_active_scene)
        {
            return;
        }

        if (get_scene()->get_viewport_height() != height)
        {
            get_scene()->set_viewport_height(height);
            m_needs_resize = true;
        }
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::begin_scene()
    {
        R_ASSERT(m_active_scene);
        R_ASSERT(!m_is_active);

        m_is_active = true;

        auto group = m_active_scene->get_registry().group<ecs::TransformComponent, ecs::CameraComponent>();
        for (auto& entity : group)
        {
            auto& camera_comp = group.get<ecs::CameraComponent>(entity);
            if (camera_comp.camera.is_active())
            {
                m_active_camera_data.camera_component = &camera_comp;
                m_active_camera_data.camera_transform_component = &group.get<ecs::TransformComponent>(entity);
            }
        }

        if (m_active_camera_data)
        {
            if (m_pipeline_setup)
            {
                if (m_needs_resize)
                {
                    m_needs_resize = false;

                    uint32 vp_width = get_viewport_width();
                    uint32 vp_height = get_viewport_height();

                    for (const std::unique_ptr<SceneRenderPass>& scenerenderpass : m_renderpasses)
                    {
                        scenerenderpass->get_pipeline()->get_render_pass()->resize(vp_width, vp_height);
                    }

                    if (!m_active_camera_data.camera_component->has_fixed_aspect_ratio)
                    {
                        m_active_camera_data.camera_component->camera.set_viewport_size(vp_width, vp_height);
                    }
                }

                Camera& active_camera = m_active_camera_data.camera_component->camera;
                Transform& active_camera_transform = m_active_camera_data.camera_transform_component->transform;

                rex::matrix4 projection = active_camera.get_projection_matrix();
                rex::matrix4 view = rex::look_at(active_camera_transform.get_position(), active_camera_transform.get_position() + active_camera_transform.get_forward(), rex::world_up<float>());

                rex::matrix4 view_projection = projection * view;
                rex::matrix4 inverse_view_projection = rex::inverse(view_projection);

                UBCamera& camera_data = m_ub_camera;

                camera_data.view_projection = view_projection;
                camera_data.inverse_view_projection = inverse_view_projection;
                camera_data.projection = projection;
                camera_data.view = view;

                Renderer::submit([camera_data]()
                                 {
                                     auto buffer = UniformBufferSet::instance()->get(CAMERA_UNIFORM_BUFFER_NAME);

                                     R_ASSERT_X(buffer, "No known Camera uniform buffer");

                                     buffer->set_data(&camera_data, sizeof(camera_data), 0u, IsRenderThread::YES);
                                 });

                setup_lights();

                for (const std::unique_ptr<SceneRenderPass>& scenerenderpass : m_renderpasses)
                {
                    if (!scenerenderpass->is_enabled())
                    {
                        continue;
                    }

                    scenerenderpass->begin(m_active_camera_data.camera_component->camera, m_active_camera_data.camera_transform_component->transform);
                }

                on_begin_scene();

                setup_entities();
            }
            else
            {
                m_clear_pass.begin(m_active_camera_data.camera_component->camera, m_active_camera_data.camera_transform_component->transform);
            }
        }
        else
        {
            m_clear_pass.begin({}, {});
        }
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::end_scene()
    {
        flush();

        if (m_pipeline_setup)
        {
            for (const std::unique_ptr<SceneRenderPass>& scenerenderpass : m_renderpasses)
            {
                if (!scenerenderpass->is_enabled())
                {
                    continue;
                }

                scenerenderpass->end();
            }

            on_end_scene();
        }
        else
        {
            m_clear_pass.end();
        }

        m_draw_list.clear();

        m_is_active = false;
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::submit_model(const ref_ptr<Model>& model)
    {
        submit_model(model, rex::matrix4(1.0));
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::submit_model(const ref_ptr<Model>& model, const rex::matrix4& transform)
    {
        submit_model(model, transform, nullptr);
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::submit_model(const ref_ptr<Model>& model, const rex::matrix4& transform, const ref_ptr<Material>& overrideMaterial)
    {
        m_draw_list.push_back({model, overrideMaterial, transform});
    }

    //-------------------------------------------------------------------------
    rex::SceneRenderPass* SceneRenderer::get_scene_render_pass(const StringID& id)
    {
        if (id == ESID::SID_None)
        {
            return nullptr;
        }

        auto it = std::find_if(std::begin(m_renderpasses), std::end(m_renderpasses), [id](std::unique_ptr<SceneRenderPass>& renderpass)
                               {
                                   return renderpass->get_name() == id;
                               });

        return it != std::cend(m_renderpasses) ? (*it).get() : nullptr;
    }
    //-------------------------------------------------------------------------
    rex::SceneRenderPass* SceneRenderer::get_previous_render_pass()
    {
        return m_previous_scenerenderpass;
    }
    //-------------------------------------------------------------------------
    rex::SceneRenderPass* SceneRenderer::get_active_render_pass()
    {
        return m_current_scenerenderpass;
    }

    //-------------------------------------------------------------------------
    const rex::SceneRenderPass* SceneRenderer::get_scene_render_pass(const StringID& id) const
    {
        if (id == ESID::SID_None)
        {
            return nullptr;
        }

        auto it = std::find_if(std::cbegin(m_renderpasses), std::cend(m_renderpasses), [id](const std::unique_ptr<SceneRenderPass>& renderpass)
                               {
                                   return renderpass->get_name() == id;
                               });

        return it != std::cend(m_renderpasses) ? (*it).get() : nullptr;
    }
    //-------------------------------------------------------------------------
    const rex::SceneRenderPasses& SceneRenderer::get_scene_render_passes() const
    {
        return m_renderpasses;
    }
    //-------------------------------------------------------------------------
    const rex::SceneRenderPass* SceneRenderer::get_previous_render_pass() const
    {
        return m_previous_scenerenderpass;
    }
    //-------------------------------------------------------------------------
    const rex::SceneRenderPass* SceneRenderer::get_active_render_pass() const
    {
        return m_current_scenerenderpass;
    }

    //-------------------------------------------------------------------------
    const int32 SceneRenderer::get_viewport_width() const
    {
        return get_scene()->get_viewport_width();
    }
    //-------------------------------------------------------------------------
    const int32 SceneRenderer::get_viewport_height() const
    {
        return get_scene()->get_viewport_height();
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<ecs::Scene>& SceneRenderer::get_scene()
    {
        return m_active_scene;
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<ecs::Scene>& SceneRenderer::get_scene() const
    {
        return m_active_scene;
    }

    //-------------------------------------------------------------------------
    const std::vector<rex::DrawCommand>& SceneRenderer::get_draw_commands() const
    {
        return m_draw_list;
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::on_begin_scene()
    {
        /* implement in derived class */
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::on_end_scene()
    {
        /* implement in derived class */
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::on_destroy()
    {
        /* implement in derived class */
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::on_begin_flush()
    {
        /* implement in derived class */
    }
    //-------------------------------------------------------------------------
    void SceneRenderer::on_end_flush()
    {
        /* implement in derived class */
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::setup_lights()
    {
        const std::vector<PointLight>& point_lights = get_scene()->get_light_environment().point_lights;

        UBPointLights& ub_point_lights = m_ub_point_lights;

        ub_point_lights.count = uint32(point_lights.size());

        std::memcpy(ub_point_lights.point_lights, point_lights.data(), sizeof(PointLight) * point_lights.size());

        Renderer::submit([&ub_point_lights]()
                         {
                             auto buffer = UniformBufferSet::instance()->get(POINTLIGHTS_UNIFORM_BUFFER_NAME);

                             R_ASSERT_X(buffer, "No known Point light uniform buffer");

                             buffer->set_data(&ub_point_lights, sizeof(ub_point_lights), 0u, IsRenderThread::YES);
                         });

        R_TODO("Setup directional lights");

        // const std::vector<DirectionalLight>& dir_lights = get_scene()->getLightEnvironment().directional_lights;
        //
        // UBDirectionalLights ub_dir_lights = m_ub_directional_lights;
        //
        // ub_dir_lights.count = uint32(dir_lights.size());
        //
        // std::memcpy(ub_dir_lights.dir_lights, dir_lights.data(), sizeof(DirectionalLight) * dir_lights.size());
        //
        // Renderer::submit([&ub_point_lights]()
        //     {
        //         auto buffer = UniformBufferSet::instance()->get(DIRECTIONALLIGHTS_UNIFORM_BUFFER_NAME);
        //
        //         R_ASSERT_X(buffer, "No known Directional light uniform buffer");
        //
        //         buffer->set_data(&ub_point_lights, sizeof(ub_point_lights), 0u, IsRenderThread::YES);
        //     });
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::setup_entities()
    {
        if (get_scene()->get_entity_count() == 0)
        {
            return;
        }

        auto view = get_scene()->get_registry().view<ecs::TransformComponent, ecs::ModelComponent, ecs::MaterialComponent>();
        for (auto entity : view)
        {
            auto& transform_comp = view.get<ecs::TransformComponent>(entity);
            auto& model_comp = view.get<ecs::ModelComponent>(entity);
            auto& material_comp = view.get<ecs::MaterialComponent>(entity);

            R_TODO("A material component should contain all materials");

            /*
                for(auto& material : material_comp.materials)
                {
                    model_comp.model->setMaterial(material->getIndex(), material);
                }
            */

            model_comp.model->set_material(0, material_comp.material);

            submit_model(model_comp.model, transform_comp.transform.get_world());
        }
    }

    //-------------------------------------------------------------------------
    void SceneRenderer::flush()
    {
        if (m_pipeline_setup)
        {
            on_begin_flush();

            for (const std::unique_ptr<SceneRenderPass>& scenerenderpass : m_renderpasses)
            {
                if (!scenerenderpass->is_enabled())
                {
                    continue;
                }

                m_previous_scenerenderpass = m_current_scenerenderpass;
                m_current_scenerenderpass = scenerenderpass.get();

                scenerenderpass->render();
            }

            m_previous_scenerenderpass = nullptr;
            m_current_scenerenderpass = nullptr;

            on_end_flush();
        }
        else
        {
            m_clear_pass.render();
        }

        TextureResourceManager::release_all();
    }
}