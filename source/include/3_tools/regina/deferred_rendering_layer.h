#pragma once

#include "layer.h"

#include "mouse_orbit_camera_controller.h"

#include "ecs/scene_camera.h"

#include "event.h"

namespace rex
{
    struct FrameInfo;

    class SceneRenderer;
    class SceneRenderPass;

    struct PreDepthPassOptions;
    struct DeferredGeometryPassOptions;
    struct DeferredLightPassOptions;
    struct CompositePassOptions;

    class CoreWindow;

    namespace ecs
    {
        class Scene;
    }
} // namespace rex

namespace regina
{
    class DeferredRenderingLayer : public rex::Layer
    {
    public:
        DeferredRenderingLayer(const rex::CoreWindow* window);
        ~DeferredRenderingLayer() override;

        void on_attach() override;
        void on_detach() override;

        void on_update(const rex::FrameInfo& info) override;
        void on_event(rex::events::Event& event) override;

    private:
        void setup_scene();
        void setup_camera();
        void setup_scene_renderer();

        std::unique_ptr<rex::SceneRenderPass> create_pre_depth_pass(const rex::PreDepthPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_geometry_pass(const rex::DeferredGeometryPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_composite_pass(const rex::CompositePassOptions& options) const;

        rex::ecs::SceneCamera m_camera;
        MouseOrbitCameraController m_camera_controller;

        rex::ref_ptr<rex::ecs::Scene> m_scene;
        rex::ref_ptr<rex::SceneRenderer> m_scene_renderer;

        const rex::CoreWindow* m_window;
    };
} // namespace regina