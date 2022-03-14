#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;

    struct DeferredGeometryPassOptions
    {
        DeferredGeometryPassOptions()
            : pass_name(ESID::SID_None)
            , geometry_shader_name("g_buffer"_sid)
            , diffuse_texture_name(ESID::SID_None)
            , specular_texture_name(ESID::SID_None)
            , backface_culling(true)
        {
        }

        StringID pass_name;
        StringID diffuse_texture_name;
        StringID specular_texture_name;
        StringID geometry_shader_name;

        bool backface_culling;
    };

    class DeferredGeometryPass : public SceneRenderPass
    {
    public:
        DeferredGeometryPass(const DeferredGeometryPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~DeferredGeometryPass() override;

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void create_geometry_pipeline();

        DeferredGeometryPassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}