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

    using ApplyToneMapping = YesNoEnum;
    using ApplyGammaCorrection = YesNoEnum;

    struct CompositePassOptions
    {
        CompositePassOptions()
            : pass_name(ESID::SID_None)
            , shader_name("blit"_sid)
            , color_buffer(ESID::SID_None)
            , gamma_correction(2.2f)
            , apply_tone_mapping(ApplyToneMapping::YES)
            , apply_gamma_correction(ApplyGammaCorrection::YES)
        {
        }

        StringID pass_name;
        StringID shader_name;

        SceneRenderPassID color_buffer;

        float gamma_correction;

        ApplyGammaCorrection apply_gamma_correction;
        ApplyToneMapping apply_tone_mapping;
    };

    class CompositePass : public SceneRenderPass
    {
    public:
        CompositePass(const CompositePassOptions& options, CreateFrameBuffer create_frame_buffer = CreateFrameBuffer::YES);
        ~CompositePass() override;

        void set_gamma(float gamma);

        void set_apply_gamma_correction(ApplyGammaCorrection apply);
        void set_apply_tone_mapping(ApplyToneMapping apply);

        void begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) override;
        void render() override;
        void end() override;

    protected:
        void on_initialize(const ref_ptr<SceneRenderer>& renderer) override;
        void on_shutdown() override;

    private:
        void set_color();

        CompositePassOptions m_options;
        CreateFrameBuffer m_create_framebuffer;
    };
}