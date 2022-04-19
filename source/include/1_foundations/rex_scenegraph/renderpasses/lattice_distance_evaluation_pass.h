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

    struct LatticeOptions
    {
        LatticeOptions()
            : lattice_strud_thickness(0.08f)
            , lattice_grid_min_bounds(-0.5f, -0.5f, -0.5f)
            , lattice_grid_max_bounds(0.5f, 0.5f, 0.5f)
            , lattice_grid_size(1.0f, 1.0f, 1.0f)
            , lattice_grid_cell_size(0.1f, 0.1f, 0.1f)
            , lattice_data_name(ESID::SID_None)
        {}

        float lattice_strud_thickness;

        rex::vec3 lattice_grid_min_bounds;
        rex::vec3 lattice_grid_max_bounds;
        rex::vec3 lattice_grid_size;
        rex::vec3 lattice_grid_cell_size;

        StringID lattice_data_name;
    };

    class LatticeDistanceEvaluationPass : public DistanceEvaluationPass
    {
    public:
        LatticeDistanceEvaluationPass(const LatticeOptions& latticeOptions, const DistanceEvaluationsPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~LatticeDistanceEvaluationPass() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void upload_lattice_options(const LatticeOptions& latticeOptions);

        LatticeOptions m_lattice_options;
    };
} // namespace rex