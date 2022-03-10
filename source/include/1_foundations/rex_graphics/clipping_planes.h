#pragma once

namespace rex
{
    struct ClippingPlanes
    {
        //-------------------------------------------------------------------------
        ClippingPlanes()
            : near_plane(0.1f)
            , far_plane(100.0f)
        {
        }
        //-------------------------------------------------------------------------
        ClippingPlanes(float nearPlane, float farPlane)
            : near_plane(nearPlane)
            , far_plane(farPlane)
        {
        }

        float near_plane;
        float far_plane;
    };
}