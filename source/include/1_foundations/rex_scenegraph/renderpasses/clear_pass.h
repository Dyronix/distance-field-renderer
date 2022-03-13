#pragma once

#include "scene_render_pass.h"
#include "color.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    struct ClearPassOptions
    {
        ClearPassOptions()
            :pass_name(ESID::SID_None)
            ,shader_name(ESID::SID_None)
            ,clear_color(colors::white)
        {}

        StringID    pass_name;
        StringID    shader_name;

        Color       clear_color;
    };

    class ClearPass : public SceneRenderPass
    {
    public:
        ClearPass(const ClearPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~ClearPass() override; 

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        ClearPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}