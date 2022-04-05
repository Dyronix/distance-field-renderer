#pragma once

#include "renderpasses/distance_evaluation_pass.h"
#include "renderpasses/distance_evaluation_pass_options.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    class HeatMapDistanceEvaluationPass : public DistanceEvaluationPass
    {
    public:
        HeatMapDistanceEvaluationPass(const StringID& colorRampName, const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~HeatMapDistanceEvaluationPass() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void upload_color_ramp_texture(const StringID& textureName);

        StringID m_color_ramp_name;
    };
} // namespace rex