#include "rex_scenegraph_pch.h"

#include "renderpasses/distance_evaluation_pass_helpers.h"

#include "aspect_ratio.h"
#include "field_of_view.h"

namespace rex
{
    namespace sdf
    {
        //-------------------------------------------------------------------------
        rex::matrix4 generate_frustrum_matrix(const rex::AspectRatio::Ratio& aspectRatio, const rex::FieldOfView& fov)
        {
            rex::matrix4 frustum_corners = rex::identity<rex::matrix4>();

            float fov_v_half = fov.get_vertical().to_rad() * 0.5f;
            float tan_fov = std::tan(fov_v_half);

            rex::vec3 to_right = rex::world_right<float>() * tan_fov * aspectRatio.get();
            rex::vec3 to_top = rex::world_up<float>() * tan_fov;

            rex::vec3 top_left = (-rex::world_forward<float>() - to_right + to_top);
            rex::vec3 top_right = (-rex::world_forward<float>() + to_right + to_top);
            rex::vec3 bottom_right = (-rex::world_forward<float>() + to_right - to_top);
            rex::vec3 bottom_left = (-rex::world_forward<float>() - to_right - to_top);

            frustum_corners = rex::column(frustum_corners, 0, rex::vec4(top_left, 0.0f));
            frustum_corners = rex::column(frustum_corners, 1, rex::vec4(top_right, 0.0f));
            frustum_corners = rex::column(frustum_corners, 2, rex::vec4(bottom_right, 0.0f));
            frustum_corners = rex::column(frustum_corners, 3, rex::vec4(bottom_left, 0.0f));

            return frustum_corners;
        }
    }
}