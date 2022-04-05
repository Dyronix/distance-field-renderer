#pragma once

namespace rex
{
    namespace sdf
    {
        rex::matrix4 generate_frustrum_matrix(const rex::AspectRatio::Ratio& aspectRatio, const rex::FieldOfView& fov);
    }
}