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
    struct DeferredGeometryPassOptions;
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
    }
} // namespace rex

namespace regina
{
    struct GeometryRenderingLayerDescription
    {
        GeometryRenderingLayerDescription()
            : source_content_location(rex::ESID::SID_None)
            , mesh_type(0)
            , nr_lights(32)
            , use_lattice(false)
            , animate(false)
        {
        }

        rex::StringID source_content_location;

        int32 mesh_type;
        int32 nr_lights;

        bool use_lattice;
        bool animate;
    };

    class GeometryRenderingLayer : public rex::Layer
    {
    public:
        GeometryRenderingLayer(const rex::CoreWindow* window, const GeometryRenderingLayerDescription& description);
        ~GeometryRenderingLayer() override;

        void on_attach() override;
        void on_detach() override;

        void on_update(const rex::FrameInfo& info) override;
        void on_event(rex::events::Event& event) override;

    private:
        bool on_key_pressed(const rex::events::KeyPressed& keyPressEvent);

        void animate_camera(const rex::FrameInfo& info);
        void read_framebuffer();
        void toggle_camera_animation();

        void setup_scene();
        void setup_camera();
        void setup_scene_renderer();

        void setup_lights();
        void setup_geometry();

        std::unique_ptr<rex::SceneRenderPass> create_deferred_geometry_pass(const rex::DeferredGeometryPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_pass(const rex::DeferredLightPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_deferred_light_visualization_pass(const rex::DeferredLightVisualizationPassOptions& options) const;
        std::unique_ptr<rex::SceneRenderPass> create_composite_pass(const rex::CompositePassOptions& options) const;

        rex::ecs::SceneCamera m_camera;
        MouseOrbitCameraController m_camera_controller;

        rex::ref_ptr<rex::ecs::Scene> m_scene;
        rex::ref_ptr<rex::SceneRenderer> m_scene_renderer;

        rex::ref_ptr<rex::Material> m_material;

        std::vector<rex::ecs::Entity> m_entities;

        const rex::CoreWindow* m_window;
    };
} // namespace regina