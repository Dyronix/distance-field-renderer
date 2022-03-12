#pragma once

#include "yes_no_enum.h"

#include "clipping_planes.h"
#include "field_of_view.h"
#include "aspect_ratio.h"

#include "transform.h"

namespace rex
{
    struct FrameInfo;

    class DegAngle;

    class PerspectiveCamera
    {
    public:
        using AdjustFOV = YesNoEnum;

        PerspectiveCamera(const FieldOfView& fieldOfView, const AspectRatio& aspectRatio, const ClippingPlanes& clippingPlanes);
        ~PerspectiveCamera();

        void set_position(const rex::vec3& position);
        void set_rotation(const rex::quaternion& rotation);

        void set_aspect_ratio(const AspectRatio& ratio, AdjustFOV adjustFOV = AdjustFOV::NO);
        void set_vertical_field_of_view(const DegAngle& angle);
        void set_horizontal_field_of_view(const DegAngle& angle);
        void set_clipping_planes(const ClippingPlanes& planes);

        const rex::vec3& get_position() const;
        const rex::quaternion& get_rotation() const;

        const AspectRatio& get_aspect_ratio() const;
        const FieldOfView& get_field_of_view() const;
        const DegAngle& get_vertical_field_of_view() const;
        const DegAngle& get_horizontal_field_of_view() const;
        const ClippingPlanes& get_clipping_planes() const;

        const rex::matrix4& get_inverse_view_projection_matrix() const;
        const rex::matrix4& get_view_projection_matrix() const;
        const rex::matrix4& get_projection_matrix() const;
        const rex::matrix4& get_view_matrix() const;
        const rex::matrix4& get_frustrum_corners() const;
        const rex::matrix4& get_world_matrix() const;

        const rex::vec3& get_up_direction() const;
        const rex::vec3& get_right_direction() const;
        const rex::vec3& get_forward_direction() const;

    protected:
        rex::matrix4 calculate_view_matrix() const;
        rex::matrix4 calculate_view_projection_matrix() const;
        rex::matrix4 calculate_frustrum_matrix() const;

        void update_matrices();

    private:
        Transform m_transform;

        FieldOfView m_field_of_view;
        AspectRatio m_aspect_ratio;
        ClippingPlanes m_clipping_planes;

        rex::matrix4 m_inv_view_projection_matrix;
        rex::matrix4 m_view_projection_matrix;
        rex::matrix4 m_projection_matrix;
        rex::matrix4 m_view_matrix;
        rex::matrix4 m_frustrum_matrix;
    };
}