#pragma once

#include "aspect_ratio.h"

namespace rex
{
    class Camera
    {
    public:
        Camera();
        Camera(const AspectRatio& aspectRatio, const rex::matrix4& projection = rex::matrix4(1.0f));
        virtual ~Camera();

        void set_viewport_size(int32 width, int32 height);

        const rex::matrix4& get_projection_matrix() const;
        const AspectRatio get_aspect_ratio() const;

        bool operator==(const Camera& other) const;
        bool operator!=(const Camera& other) const;

    protected:
        void set_projection_matrix(const rex::matrix4& projection);

        virtual void on_set_viewport_size(int32 width, int32 height) = 0;

    private:
        rex::matrix4 m_projection;
        AspectRatio m_aspect_ratio;
    };
}