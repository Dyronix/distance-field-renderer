#pragma once

#include "layer.h"

#include "mouse_orbit_camera_controller.h"

#include "ecs/scene_camera.h"

#include "event.h"

namespace rex
{
    class SceneRenderer;
    class SceneRenderPass;
    class Material;
    class CoreWindow;

    struct FrameInfo;
    struct PreDepthPassOptions;
    struct DistanceEvaluationsPassOptions;
    struct DeferredLightPassOptions;
    struct DeferredLightVisualizationPassOptions;
    struct CompositePassOptions;

    namespace ecs
    {
        class Scene;
        class Entity;
    } // namespace ecs
} // namespace rex

namespace regina
{
    class DistanceFieldRenderingLayer : public rex::Layer
    {
    public:
        DistanceFieldRenderingLayer(const rex::CoreWindow* window);
        ~DistanceFieldRenderingLayer() override;

        void on_attach() override;
        void on_detach() override;

        void on_update(const rex::FrameInfo& info) override;
        void on_event(rex::events::Event& event) override;

    private:
        void setup_scene();
        void setup_camera();
        void setup_scene_renderer();

        void setup_lights();
        void setup_geometry();

        std::unique_ptr<rex::SceneRenderPass> create_pre_depth_pass(const rex::PreDepthPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_distance_evaluation_pass(const rex::DistanceEvaluationsPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_composite_pass(const rex::CompositePassOptions& options) const;

        rex::ecs::SceneCamera m_camera;
        MouseOrbitCameraController m_camera_controller;

        rex::ref_ptr<rex::ecs::Scene> m_scene;
        rex::ref_ptr<rex::SceneRenderer> m_scene_renderer;

        rex::ref_ptr<rex::Material> m_bunny_material;

        std::vector<rex::ecs::Entity> m_bunny_entities;

        const rex::CoreWindow* m_window;
    };
} // namespace regina