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
    struct PreDepthPassOptions;
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
    struct DistanceFieldRenderingLayerDescription
    {
        rex::StringID source_content_location;

        int32 volume_type;
        int32 max_iterations;
        int32 nr_lights;
        int32 resolution;

        bool use_lattice;
        bool use_heatmap;
    };

    class DistanceFieldRenderingLayer : public rex::Layer
    {
    public:
        DistanceFieldRenderingLayer(const rex::CoreWindow* window, const DistanceFieldRenderingLayerDescription& description);
        ~DistanceFieldRenderingLayer() override;

        void on_attach() override;
        void on_detach() override;

        void on_update(const rex::FrameInfo& info) override;
        void on_event(rex::events::Event& event) override;

    private:
        bool on_key_pressed(const rex::events::KeyPressed& keyPressEvent);

        void animate_camera(const rex::FrameInfo& info);
        void read_framebuffer();

        void decrement_sdf_scale();
        void decrement_sdf_offset();
        void increment_sdf_scale();
        void increment_sdf_offset();

        void switch_to_heatmap();
        void switch_to_sdf();

        void setup_scene();
        void setup_camera();
        void setup_scene_renderer();
        void setup_sdf_renderer();
        void setup_heatmap_renderer();

        void setup_lights();

        std::unique_ptr<rex::SceneRenderPass> create_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_heatmap_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_composite_pass(const rex::CompositePassOptions& options) const;

        rex::ecs::SceneCamera m_camera;
        MouseOrbitCameraController m_camera_controller;

        rex::ref_ptr<rex::ecs::Scene> m_scene;

        rex::ref_ptr<rex::SceneRenderer> m_heatmap_renderer;
        rex::ref_ptr<rex::SceneRenderer> m_sdf_renderer;

        rex::SceneRenderer* m_active_renderer;

        const rex::CoreWindow* m_window;
    };
} // namespace regina