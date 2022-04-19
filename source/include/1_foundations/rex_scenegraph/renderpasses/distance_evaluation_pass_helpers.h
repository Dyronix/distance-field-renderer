#pragma once

namespace rex
{
    class AspectRatio;
    class FieldOfView;

    namespace sdf
    {
        rex::matrix4 generate_frustrum_matrix(const rex::AspectRatio& aspectRatio, const rex::FieldOfView& fov);
    }
}