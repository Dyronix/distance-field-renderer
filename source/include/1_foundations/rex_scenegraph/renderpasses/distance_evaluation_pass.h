#pragma once

#include "scene_render_pass.h"

#include "renderpasses/distance_evaluation_pass_options.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    struct DistanceEvaluationsPassOptions
    {
        DistanceEvaluationsPassOptions()
            : pass_name(ESID::SID_None)
            , shader_name("distance_evaluation"_sid)
        {
        }

        StringID pass_name;
        StringID shader_name;

        sdf::SphereTracerOptions sphere_tracer_options;
        sdf::SceneOptions sdf_scene_options;
    };

    class DistanceEvaluationPass : public SceneRenderPass
    {
    public:
        DistanceEvaluationPass(const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~DistanceEvaluationPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

        void set_sphere_tracer_options(const sdf::SphereTracerOptions& sphereTracerOptions);
        void set_sdf_scene_options(const sdf::SceneOptions& sdfSceneOptions);

        const sdf::SphereTracerOptions& get_sphere_tracer_options() const;
        const sdf::SceneOptions& get_sdf_scene_options() const;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

        void upload_sphere_tracer_options(const sdf::SphereTracerOptions& sphereTracerOptions);
        void upload_sdf_scene_options(const sdf::SceneOptions& sdfSceneOptions);

        const DistanceEvaluationsPassOptions& get_options();

        bool should_create_frame_buffer() const;

    private:
        DistanceEvaluationsPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}