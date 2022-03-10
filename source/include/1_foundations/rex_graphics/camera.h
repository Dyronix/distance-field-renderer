#pragma once

#include "aspectratio.h"

namespace rex
{
    class Camera
    {
    public:
        Camera();
        Camera(const AspectRatio& aspectRatio, const rex::matrix4& projection = rex::matrix4(1.0f));
        virtual ~Camera();

        void setViewportSize(int32 width, int32 height);

        const rex::matrix4& getProjectionMatrix() const;
        const AspectRatio::Ratio getAspectRatio() const;

        bool operator==(const Camera& other) const;
        bool operator!=(const Camera& other) const;

    protected:
        void setProjectionMatrix(const rex::matrix4& projection);

        virtual void onSetViewportSize(int32 width, int32 height) = 0;

    private:
        rex::matrix4 m_projection;
        AspectRatio m_aspect_ratio;
    };
}