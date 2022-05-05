#pragma once

#include "win32_application.h"

namespace rex
{
    struct FrameInfo;

    class Layer;
} // namespace rex

namespace regina
{
    class Application : public rex::win32::Application
    {
    public:
        Application(const rex::ApplicationDescription& description);
        ~Application() override;

        void on_app_initialize() override;
        void on_app_update(const rex::FrameInfo& info) override;

    private:
        rex::Layer* m_geometry_layer;
        rex::Layer* m_volume_layer;
        rex::Layer* m_shotcut_layer;

        float m_application_time;
        int32 m_frame_counter;
    };
}