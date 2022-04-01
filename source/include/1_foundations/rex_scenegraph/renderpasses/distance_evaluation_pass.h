#pragma once

#include "scene_render_pass.h"

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