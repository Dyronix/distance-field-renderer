#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    struct BlurPassOptions
    {
        BlurPassOptions()
            :pass_name(ESID::SID_None)
            ,shader_name("blur"_sid)
            ,color_buffer(ESID::SID_None)
            ,blur_width(4)
            ,blur_height(4)
        {}

        StringID            pass_name;
        StringID            shader_name;

        SceneRenderPassID   color_buffer;

        int32               blur_width;
        int32               blur_height;
    };

    class BlurPass : public SceneRenderPass
    {
    public:
        BlurPass(const BlurPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~BlurPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void setColor();

        BlurPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}