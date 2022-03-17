#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;
    class Texture;
    class Model;

    struct PointLight;

    struct DeferredLightVisualizationPassOptions
    {
        DeferredLightVisualizationPassOptions()
            : pass_name(ESID::SID_None)
            , color_pass_name(ESID::SID_None)
            , depth_pass_name(ESID::SID_None)
            , shader_name("deferred_light_visualization"_sid)
        {
        }

        StringID pass_name;
        StringID color_pass_name;
        StringID depth_pass_name;
        StringID shader_name;
    };

    class DeferredLightVisualizationPass : public SceneRenderPass
    {
    public:
        DeferredLightVisualizationPass(const DeferredLightVisualizationPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~DeferredLightVisualizationPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        DeferredLightVisualizationPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
        ref_ptr<Model> m_unit_cube;
    };
} // namespace rex