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
}

namespace regina
{
    struct LatticeOptions
    {
        LatticeOptions()
            : lattice_strud_thickness(0.08f)
            , lattice_grid_min_bounds(-0.5f, -0.5f, -0.5f)
            , lattice_grid_max_bounds(0.5f, 0.5f, 0.5f)
            , lattice_grid_size(1.0f, 1.0f, 1.0f)
            , lattice_grid_cell_size(0.1f, 0.1f, 0.1f)
            , lattice_data_name(rex::ESID::SID_None)
        {}

        float lattice_strud_thickness;

        rex::vec3 lattice_grid_min_bounds;
        rex::vec3 lattice_grid_max_bounds;
        rex::vec3 lattice_grid_size;
        rex::vec3 lattice_grid_cell_size;

        rex::StringID lattice_data_name;
    };

    class LatticeDistanceEvaluationPass : public rex::DistanceEvaluationPass
    {
    public:
        LatticeDistanceEvaluationPass(const LatticeOptions& latticeOptions, const rex::DistanceEvaluationsPassOptions& options, rex::CreateFrameBuffer create_frame_buffer = rex::CreateFrameBuffer::YES);
        ~LatticeDistanceEvaluationPass() override;

    protected:
        void on_initialize(const rex::ref_ptr<rex::SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void upload_lattice_options(const LatticeOptions& latticeOptions);

        LatticeOptions m_lattice_options;
    };
} // namespace rex