#pragma once

#include "renderpasses/clear_pass.h"

#include "renderer/point_light.h"
#include "renderer/directional_light.h"

namespace rex
{
    struct Environment;

    class SceneRenderPass;
    class PerspectiveCamera;
    class Model;
    class Material;

    namespace ecs
    {
        class Scene;

        struct CameraComponent;
        struct TransformComponent;
    }

    struct DrawCommand
    {
        ref_ptr<Model> model;
        ref_ptr<Material> override_material;

        rex::matrix4 transform;
    };

    using SceneRenderPasses = std::vector<std::unique_ptr<SceneRenderPass>>;
    using DrawCommands = std::vector<DrawCommand>;

    class SceneRenderer : public RefCountedObject
    {
    public:
        SceneRenderer(ref_ptr<ecs::Scene> scene, SceneRenderPasses&& renderPasses);
        ~SceneRenderer();

        void destroy();

        void set_scene(const ref_ptr<ecs::Scene>& scene);
        void set_viewport_width(int32 width);
        void set_viewport_height(int32 height);

        void begin_scene();
        void end_scene();

        void submit_model(const ref_ptr<Model>& model);
        void submit_model(const ref_ptr<Model>& model, const rex::matrix4& transform);
        void submit_model(const ref_ptr<Model>& model, const rex::matrix4& transform, const ref_ptr<Material>& overrideMaterial);

        SceneRenderPass* get_scene_render_pass(const StringID& id);
        SceneRenderPass* get_previous_render_pass();
        SceneRenderPass* get_active_render_pass();

        const SceneRenderPass* get_scene_render_pass(const StringID& id) const;
        const SceneRenderPasses& get_scene_render_passes() const;
        const SceneRenderPass* get_previous_render_pass() const;
        const SceneRenderPass* get_active_render_pass() const;

        const int32 get_viewport_width() const;
        const int32 get_viewport_height() const;

        ref_ptr<ecs::Scene>& get_scene();
        const ref_ptr<ecs::Scene>& get_scene() const;

        const std::vector<DrawCommand>& get_draw_commands() const;

    protected:
        virtual void on_begin_scene();
        virtual void on_end_scene();
        virtual void on_destroy();
        virtual void on_begin_flush();
        virtual void on_end_flush();

    private:
        void setup_entities();

        void flush();

        struct ActiveCameraData
        {
            ActiveCameraData()
                : camera_component(nullptr)
                , camera_transform_component(nullptr)
            {
            }

            operator bool()
            {
                return camera_component != nullptr && camera_transform_component != nullptr;
            }

            ecs::CameraComponent* camera_component;
            ecs::TransformComponent* camera_transform_component;
        };

        struct UBCamera
        {
            rex::matrix4 view_projection;
            rex::matrix4 inverse_view_projection;
            rex::matrix4 projection;
            rex::matrix4 view;
        } m_ub_camera;

        struct UBDirectionalLights
        {
            uint32 count{0};
            uint32 padding{0};
            DirectionalLight dir_lights[4]{};
        } m_ub_directional_lights;

        struct UBPointLights
        {
            uint32 count{0};
            uint32 padding{0};
            PointLight point_lights[1024]{};
        } m_ub_point_lights;

        SceneRenderPass* m_previous_scenerenderpass;
        SceneRenderPass* m_current_scenerenderpass;

        ClearPass m_clear_pass;
        SceneRenderPasses m_renderpasses;
        DrawCommands m_draw_list;

        ref_ptr<ecs::Scene> m_active_scene;

        bool m_pipeline_setup;
        bool m_is_active;
        bool m_needs_resize;

        ActiveCameraData m_active_camera_data;
    };
}