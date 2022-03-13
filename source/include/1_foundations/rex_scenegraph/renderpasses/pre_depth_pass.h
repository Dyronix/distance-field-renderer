#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    struct PreDepthPassOptions
    {
        PreDepthPassOptions()
            :pass_name(ESID::SID_None)
            ,shader_name("predepth"_sid)
            ,backface_culling(true)
            ,near_plane(0.1f)
            ,far_plane(1000.0)
        {}

        StringID    pass_name;
        StringID    shader_name;

        bool        backface_culling;

        float       near_plane;
        float       far_plane;
    };

    class PreDepthPass : public SceneRenderPass
    {
    public:
        PreDepthPass(const PreDepthPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~PreDepthPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        PreDepthPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}