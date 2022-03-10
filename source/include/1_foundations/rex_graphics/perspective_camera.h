#pragma once

#include "yesnoenum.h"

#include "clippingplanes.h"
#include "fieldofview.h"
#include "aspectratio.h"

#include "degangle.h"
#include "transform.h"

namespace rex
{
    struct FrameInfo;

    class PerspectiveCamera
    {
    public:
        using AdjustFOV = YesNoEnum;

        PerspectiveCamera(const FieldOfView& fieldOfView, const AspectRatio& aspectRatio, const ClippingPlanes& clippingPlanes);
        ~PerspectiveCamera();

        void setPosition(const rex::vec3& position);
        void setRotation(const rex::quaternion& rotation);

        void setAspectRatio(const AspectRatio& ratio, AdjustFOV adjustFOV = AdjustFOV::NO);
        void setVerticalFieldOfView(const DegAngle& angle);
        void setHorizontalFieldOfView(const DegAngle& angle);
        void setClippingPlanes(const ClippingPlanes& planes);

        const rex::vec3& getPosition() const;
        const rex::quaternion& getRotation() const;

        const AspectRatio& getAspectRatio() const;
        const FieldOfView& getFieldOfView() const;
        const DegAngle& getVerticalFieldOfView() const;
        const DegAngle& getHorizontalFieldOfView() const;
        const ClippingPlanes& getClippingPlanes() const;

        const rex::matrix4& getInverseViewProjectionMatrix() const;
        const rex::matrix4& getViewProjectionMatrix() const;
        const rex::matrix4& getProjectionMatrix() const;
        const rex::matrix4& getViewMatrix() const;
        const rex::matrix4& getFrustrumCorners() const;
        const rex::matrix4& getWorldMatrix() const;

        const rex::vec3& getUpDirection() const;
        const rex::vec3& getRightDirection() const;
        const rex::vec3& getForwardDirection() const;

    protected:
        rex::matrix4 calculateViewMatrix() const;
        rex::matrix4 calculateViewProjectionMatrix() const;
        rex::matrix4 calculateFrustrumMatrix() const;

        void updateMatrices();

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