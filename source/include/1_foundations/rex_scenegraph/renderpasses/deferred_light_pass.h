#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;
    class Texture;

    struct DeferredLightPassOptions
    {
        DeferredLightPassOptions()
            : pass_name(ESID::SID_None)
            , shader_name("blit"_sid)
            , g_position_buffer(ESID::SID_None)
            , g_normal_buffer(ESID::SID_None)
            , g_albedo_spec_buffer(ESID::SID_None)
        {
        }

        StringID pass_name;
        StringID shader_name;

        SceneRenderPassID g_position_buffer;
        SceneRenderPassID g_normal_buffer;
        SceneRenderPassID g_albedo_spec_buffer;
    };

    class DeferredLightPass : public SceneRenderPass
    {
    public:
        DeferredLightPass(const DeferredLightPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~DeferredLightPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void set_g_position();
        void set_g_normal();
        void set_g_albedo_spec();

        DeferredLightPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}