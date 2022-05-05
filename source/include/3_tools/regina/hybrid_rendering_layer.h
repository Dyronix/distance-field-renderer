#pragma once

#include "layer.h"

#include "mouse_orbit_camera_controller.h"

#include "ecs/scene_camera.h"

#include "event.h"

namespace rex
{
    class SceneRenderer;
    class SceneRenderPass;
    class CoreWindow;

    struct FrameInfo;
    struct DeferredGeometryPassOptions;
    struct DistanceEvaluationsPassOptions;
    struct DeferredLightPassOptions;
    struct DeferredLightVisualizationPassOptions;
    struct CompositePassOptions;

    namespace events
    {
        class KeyPressed;
    } // namespace events

    namespace ecs
    {
        class Scene;
        class Entity;
    } // namespace ecs
} // namespace rex

namespace regina
{
    struct HybridRenderingLayerDescription
    {
        HybridRenderingLayerDescription()
            : source_content_location(rex::ESID::SID_None)
            , mesh_type(0)
            , volume_type(0)
            , max_iterations(200)
            , nr_lights(32)
            , max_marching_distance(1000.0f)
            , min_marching_distance(0.01f)
            , use_lattice(false)
            , use_heatmap(false)
            , animate(false)
        {
        }

        rex::StringID source_content_location;

        int32 mesh_type;
        int32 volume_type;
        int32 max_iterations;
        int32 nr_lights;

        float max_marching_distance;
        float min_marching_distance;

        bool use_lattice;
        bool use_heatmap;
        bool animate;
    };

    class HybridRenderingLayer : public rex::Layer
    {
    public:
        HybridRenderingLayer(const rex::CoreWindow* window, const HybridRenderingLayerDescription& description);
        ~HybridRenderingLayer() override;

        void on_attach() override;
        void on_detach() override;

        void on_update(const rex::FrameInfo& info) override;
        void on_event(rex::events::Event& event) override;

    private:
        bool on_key_pressed(const rex::events::KeyPressed& keyPressEvent);

        void animate_camera(const rex::FrameInfo& info);
        void toggle_camera_animation();

        void decrement_sdf_scale();
        void decrement_sdf_offset();
        void increment_sdf_scale();
        void increment_sdf_offset();

        void switch_to_heatmap();
        void switch_to_sdf();

        void setup_scene();
        void setup_hybrid_scene();
        void setup_offscreen_scene();
        void setup_camera();
        void setup_scene_renderer();
        void setup_hybrid_renderer();
        void setup_heatmap_renderer();
        void setup_offscreen_renderer();

        void setup_lights();
        void setup_geometry();

        std::unique_ptr<rex::SceneRenderPass> create_deferred_geometry_pass(const rex::DeferredGeometryPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_heatmap_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_composite_pass(const rex::CompositePassOptions& options) const;

        rex::ecs::SceneCamera m_camera;
        MouseOrbitCameraController m_camera_controller;

        std::vector<rex::ecs::Entity> m_entities;

        rex::ref_ptr<rex::Material> m_material;

        rex::ref_ptr<rex::ecs::Scene> m_scene;
        rex::ref_ptr<rex::ecs::Scene> m_offscreen_scene;

        rex::ref_ptr<rex::SceneRenderer> m_hybrid_renderer;
        rex::ref_ptr<rex::SceneRenderer> m_heatmap_renderer;
        rex::ref_ptr<rex::SceneRenderer> m_offscreen_renderer;

        rex::SceneRenderer* m_active_renderer;

        const rex::CoreWindow* m_window;
    };
} // namespace regina