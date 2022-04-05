#pragma once

namespace rex
{
    class Texture;

    namespace sdf
    {
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

        struct SceneOptions
        {
            SceneOptions()
                : scene_scale(1.0f)
                , scene_offset(0.0f)
                , scene_size(100.0f, 100.0f, 100.0f)
                , scene_center(0.0f, 0.0f, 0.0f)
                , scene_voxel_grid_min_bounds(0.0f, 0.0f, 0.0f)
                , scene_voxel_grid_max_bounds(0.0f, 0.0f, 0.0f)
                , scene_voxel_grid_size(0.0f, 0.0f, 0.0f)
                , scene_voxel_grid_cell_size(0.0f, 0.0f, 0.0f)
                , scene_data(nullptr)
            {
            }

            float scene_scale;
            float scene_offset;

            rex::vec3 scene_size;
            rex::vec3 scene_center;

            rex::vec3 scene_voxel_grid_min_bounds;
            rex::vec3 scene_voxel_grid_max_bounds;
            rex::vec3 scene_voxel_grid_size;
            rex::vec3 scene_voxel_grid_cell_size;

            ref_ptr<Texture> scene_data;
        };
    } // namespace sdf
}