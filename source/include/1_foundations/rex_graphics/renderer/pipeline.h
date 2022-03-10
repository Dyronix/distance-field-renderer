#pragma once

#include "memory/ref_counted_object.h"

#include "resources/shader_program.h"
#include "resources/buffer_layout.h"

#include "renderer/states/depth_test_state.h"
#include "renderer/states/face_cull_state.h"
#include "renderer/states/rasterizer_state.h"
#include "renderer/types/primitive_type.h"
#include "renderer/render_pass.h"

#include "string/string_id.h"

namespace rex
{
    struct PipelineDescription
    {
        PipelineDescription()
            : shader(nullptr)
            , renderpass(nullptr)
            , layout()
            , primitive_type(PrimitiveType::TRIANGLES)
            , facecull_state(FaceCullingEnabled::NO)
            , depth_test_state(DepthTestEnabled::NO)
            , rasterizer_state()
            , name(EName::SID_None)
        {
        }

        ref_ptr<ShaderProgram> shader;
        ref_ptr<RenderPass> renderpass;

        BufferLayout layout;

        PrimitiveType primitive_type;
        FaceCullState facecull_state;
        DepthTestState depth_test_state;
        RasterizerState rasterizer_state;

        StringID name;
    };

    class Pipeline : public Resource
    {
    public:
        Pipeline() = default;
        virtual ~Pipeline() = default;

        virtual ref_ptr<ShaderProgram>& get_shader_program() = 0;
        virtual const ref_ptr<ShaderProgram>& get_shader_program() const = 0;
        virtual ref_ptr<RenderPass>& get_render_pass() = 0;
        virtual const ref_ptr<RenderPass>& get_render_pass() const = 0;

        virtual const BufferLayout& get_buffer_layout() const = 0;

        virtual const PrimitiveType& get_primitive_type() const = 0;
        virtual const FaceCullState& get_face_cull_state() const = 0;
        virtual const DepthTestState& get_depth_test_state() const = 0;
        virtual const RasterizerState& get_rasterizer_state() const = 0;

        virtual const StringID& get_debug_name() const = 0;

        virtual void invalidate() = 0;
        virtual void release() = 0;

        // TEMP: remove this when render command buffers are a thing
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    };
}