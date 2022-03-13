#pragma once

#include "scene_render_pass.h"

namespace rex
{
    namespace ecs
    {
        class SceneCamera;
    }

    class SceneRenderer;
    class Model;

    struct GeometryPassOptions
    {
        GeometryPassOptions()
            :pass_name(ESID::SID_None)
            ,geometry_shader_name("pbr_static"_sid)
            ,grid_shader_name("grid"_sid)
            ,skybox_shader_name("skybox"_sid)
            ,show_environment(false)
            ,show_grid(false)
            ,backface_culling(true)
            ,grid_scale(16.025f)
            ,grid_size(0.025f)
        {}

        StringID pass_name;
        StringID geometry_shader_name;
        StringID grid_shader_name;
        StringID skybox_shader_name;

        bool show_environment;
        bool show_grid;
        bool backface_culling;

        float grid_scale;
        float grid_size;
    };

    class GeometryPass : public SceneRenderPass
    {
    public:
        GeometryPass(const GeometryPassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~GeometryPass() override;

        void enable_grid();
        void disable_grid();
        void toggle_grid();

        void enable_skybox();
        void disable_skybox();
        void toggle_skybox();

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void create_geometry_pipeline();
        void create_sky_box_pipeline();
        void create_grid_pipeline();

        StringID                m_active_environment;
        
        GeometryPassOptions     m_options;
        CreateFrameBuffer       m_create_framebuffer;

        ref_ptr<Model>          m_sky_model;
    };
}