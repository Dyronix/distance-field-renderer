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
    struct LatticeOptions;

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
    class LatticeRenderingLayer : public rex::Layer
    {
    public:
        LatticeRenderingLayer(const rex::CoreWindow* window);
        ~LatticeRenderingLayer() override;

        void on_attach() override;
        void on_detach() override;

        void on_update(const rex::FrameInfo& info) override;
        void on_event(rex::events::Event& event) override;

    private:
        bool on_key_pressed(const rex::events::KeyPressed& keyPressEvent);

        void decrement_sdf_scale();
        void decrement_sdf_offset();
        void increment_sdf_scale();
        void increment_sdf_offset();

        void setup_scene();
        void setup_camera();
        void setup_scene_renderer();

        void setup_lights();

        std::unique_ptr<rex::SceneRenderPass> create_distance_evaluation_pass(const rex::LatticeOptions& latticeOptions, const rex::DistanceEvaluationsPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_composite_pass(const rex::CompositePassOptions& options) const;

        rex::ecs::SceneCamera m_camera;
        MouseOrbitCameraController m_camera_controller;

        rex::ref_ptr<rex::ecs::Scene> m_scene;
        rex::ref_ptr<rex::SceneRenderer> m_scene_renderer;

        const rex::CoreWindow* m_window;
    };
} // namespace regina