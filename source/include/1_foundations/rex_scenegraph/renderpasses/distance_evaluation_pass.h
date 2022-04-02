#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    struct SphereTracerOptions
    {
        SphereTracerOptions()
            : max_iterations(100)
            , max_march_distance(1000.0f)
            , min_surface_distance(0.1f)
        {
        }

        int32 max_iterations;
        
        float max_march_distance;
        float min_surface_distance;
    };

    struct SDFSceneOptions
    {
        SDFSceneOptions()
            : scene_size(100.0f, 100.0f, 100.0f)
            , scene_center(0.0f, 0.0f, 0.0f)
            , scene_data(nullptr)
        {
        }

        rex::vec3 scene_size;
        rex::vec3 scene_center;

        ref_ptr<Texture> scene_data;
    };
    
    struct DistanceEvaluationsPassOptions
    {
        DistanceEvaluationsPassOptions()
            : pass_name(ESID::SID_None)
            , shader_name("distance_evaluation"_sid)
        {
        }

        StringID pass_name;
        StringID shader_name;

        SphereTracerOptions sphere_tracer_options;
        SDFSceneOptions sdf_scene_options;
    };

    class DistanceEvaluationPass : public SceneRenderPass
    {
    public:
        DistanceEvaluationPass(const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~DistanceEvaluationPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        DistanceEvaluationsPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}