#include "rex_windows_pch.h"

#include "graphics/renderer/gl_renderer_api.h"
#include "graphics/renderer/gl_render_pass.h"
#include "graphics/renderer/gl_render_pipeline.h"

#include "graphics/resources/gl_texture_resource_manager.h"

#include "renderer/types/primitive_type.h"
#include "renderer/renderer.h"

#include "resources/texture_resource_manager.h"
#include "resources/resource_factory.h"
#include "resources/uniform_buffer_set.h"
#include "resources/vertex_buffer.h"
#include "resources/index_buffer.h"
#include "resources/material.h"
#include "resources/shader_program.h"
#include "resources/frame_buffer_pool.h"
#include "resources/frame_buffer.h"

#include "model.h"
#include "triangle.h"
#include "submesh.h"
#include "mesh_factory.h"

#include <map>

namespace rex
{
    namespace opengl
    {
        std::vector<ref_ptr<Pipeline>> g_pipelines;
        std::vector<ref_ptr<RenderPass>> g_renderpasses;

        ref_ptr<VertexBuffer> g_fullscreen_quad_vertex_buffer;
        ref_ptr<IndexBuffer> g_fullscreen_quad_index_buffer;

        ref_ptr<rex::RenderPass> g_active_renderpass;

        //-------------------------------------------------------------------------
        GLenum to_gl_primitive_type(const PrimitiveType& type)
        {
            switch (type)
            {
                case PrimitiveType::POINTS: return GL_POINTS;

                case PrimitiveType::LINES: return GL_LINES;
                case PrimitiveType::LINE_STRIP: return GL_LINE_STRIP;
                case PrimitiveType::LINE_LOOP: return GL_LINE_LOOP;

                case PrimitiveType::TRIANGLES: return GL_TRIANGLES;
                case PrimitiveType::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
                case PrimitiveType::TRIANGLE_FAN: return GL_TRIANGLE_FAN;

                case PrimitiveType::QUADS: return GL_QUADS;
            }

            R_ASSERT_X(false, "Invalid primitive type");
            return 0;
        }
        //-------------------------------------------------------------------------
        GLenum to_gl_cull_type(const CullMode& cullMode)
        {
            switch (cullMode)
            {
                case CullMode::BACK: return GL_BACK;
                case CullMode::FRONT: return GL_FRONT;
                case CullMode::FRONT_AND_BACK: return GL_FRONT_AND_BACK;

                default: R_ASSERT_X(false, "Unsupported Cull Type"); return GL_BACK;
            }
        }
        //-------------------------------------------------------------------------
        GLenum to_gl_winding_type(const FaceWinding& winding)
        {
            switch (winding)
            {
                case FaceWinding::CLOCKWISE: return GL_CW;
                case FaceWinding::COUNTER_CLOCKWISE: return GL_CCW;

                default: R_ASSERT_X(false, "Unsupported winding Type"); return GL_CCW;
            }
        }
        //-------------------------------------------------------------------------
        GLenum to_gl_depth_func(const DepthTestFunction& function)
        {
            switch (function)
            {
                case DepthTestFunction::ALWAYS: return GL_ALWAYS;
                case DepthTestFunction::NEVER: return GL_NEVER;
                case DepthTestFunction::LESS: return GL_LESS;
                case DepthTestFunction::LESS_OR_EQUAL: return GL_LEQUAL;
                case DepthTestFunction::GREATER: return GL_GREATER;
                case DepthTestFunction::GREATER_OR_EQUAL: return GL_GEQUAL;
                case DepthTestFunction::EQUAL: return GL_EQUAL;
                case DepthTestFunction::NOT_EQUAL: return GL_NOTEQUAL;
                default: R_ASSERT_X(false, "Unsupported depth function"); return GL_LESS;
            }
        }
        //-------------------------------------------------------------------------
        GLenum to_gl_fill_mode(FillMode mode)
        {
            switch (mode)
            {
                case FillMode::POINT: return GL_POINT;
                case FillMode::LINE: return GL_LINE;
                case FillMode::FILL: return GL_FILL;
                default: R_ASSERT_X(false, "Unsupported fill mode"); return GL_FILL;
            }
        }

        //-------------------------------------------------------------------------
        void setup_face_cull_state(const rex::FaceCullState& faceCullState)
        {
            Renderer::submit([faceCullState]()
                             {
                                 if (faceCullState.enabled)
                                 {
                                     opengl::enable(GL_CULL_FACE);
                                     opengl::cull_face(to_gl_cull_type(faceCullState.cull_mode));
                                     opengl::front_face(to_gl_winding_type(faceCullState.face_winding));
                                 }
                                 else
                                 {
                                     opengl::disable(GL_CULL_FACE);
                                 }
                             });
        }
        //-------------------------------------------------------------------------
        void setup_depth_test_state(const rex::DepthTestState& depthTestState)
        {
            Renderer::submit([depthTestState]()
                             {
                                 if (depthTestState.enabled)
                                 {
                                     opengl::enable(GL_DEPTH_TEST);
                                     opengl::depth_func(to_gl_depth_func(depthTestState.function));
                                     if (depthTestState.read_only)
                                     {
                                         opengl::depth_mask(GL_FALSE);
                                     }
                                     else
                                     {
                                         opengl::depth_mask(GL_TRUE);
                                     }
                                 }
                                 else
                                 {
                                     opengl::disable(GL_DEPTH_TEST);
                                 }
                             });
        }
        //-------------------------------------------------------------------------
        void setup_rasterizer_state(const rex::RasterizerState& rasterizerState)
        {
            Renderer::submit([rasterizerState]()
                             {
                                 R_ASSERT(rasterizerState.cull_mode == CullMode::FRONT_AND_BACK);

                                 opengl::polygon_mode(GL_FRONT_AND_BACK, to_gl_fill_mode(rasterizerState.fill_mode));
                             });
        }

        //-------------------------------------------------------------------------
        void load_fullscreen_quad()
        {
            float x = -1;
            float y = -1;
            float width = 2;
            float height = 2;

            struct QuadVertex
            {
                rex::vec3 position;
                rex::vec2 texCoord;
            };

            constexpr uint32 quad_vertex_count = 4u;

            // z coordinate is used as the index of the vertex
            QuadVertex* vertices = new QuadVertex[quad_vertex_count];

            vertices[0].position = rex::vec3(x, y, 3.0f);  // BL
            vertices[0].texCoord = rex::vec2(0, 0);

            vertices[1].position = rex::vec3(x + width, y, 2.0f);  // BR
            vertices[1].texCoord = rex::vec2(1, 0);

            vertices[2].position = rex::vec3(x + width, y + height, 1.0f);  // TR
            vertices[2].texCoord = rex::vec2(1, 1);

            vertices[3].position = rex::vec3(x, y + height, 0.0f);  // TL
            vertices[3].texCoord = rex::vec2(0, 1);

            g_fullscreen_quad_vertex_buffer = ResourceFactory::create_vertex_buffer(vertices, sizeof(QuadVertex) * quad_vertex_count, quad_vertex_count);

            delete[] vertices;  // copied into the vertex buffer

            constexpr uint32 quad_triangle_count = 2u;

            TriangleIndices indices[quad_triangle_count] = {{0, 1, 2}, {2, 3, 0}};

            g_fullscreen_quad_index_buffer = ResourceFactory::create_index_buffer(indices, quad_triangle_count);
        }
        //-------------------------------------------------------------------------
        void destroy_fullscreen_quad()
        {
            g_fullscreen_quad_vertex_buffer->release();
            g_fullscreen_quad_vertex_buffer.reset();
            g_fullscreen_quad_index_buffer->release();
            g_fullscreen_quad_index_buffer.reset();
        }

        //-------------------------------------------------------------------------
        void load_texture_resource_manager()
        {
            int32 max_texture_units = 0;
            opengl::get_integer_value(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);

            TextureResourceManager::intialize(std::make_unique<opengl::TextureResourceManagerAPI>(max_texture_units));
        }
        //-------------------------------------------------------------------------
        void destroy_texture_resource_manager()
        {
            TextureResourceManager::release_all();
        }

        //-------------------------------------------------------------------------
        RendererAPI::RendererAPI()
        {
        }
        //-------------------------------------------------------------------------
        RendererAPI::~RendererAPI()
        {
        }

        //-------------------------------------------------------------------------
        void RendererAPI::initialize()
        {
            load_texture_resource_manager();
            load_fullscreen_quad();
        }

        //-------------------------------------------------------------------------
        void RendererAPI::shutdown()
        {
            destroy_fullscreen_quad();
            destroy_texture_resource_manager();

            for (ref_ptr<Pipeline>& pipeline : g_pipelines)
            {
                pipeline->release();
            }
            g_pipelines.clear();

            g_renderpasses.clear();
        }

        //-------------------------------------------------------------------------
        void RendererAPI::set_viewport(const rex::vec2& origin, const rex::vec2& size)
        {
            Renderer::submit([origin, size]()
                             {
                                 opengl::viewport((int32)origin.x, (int32)origin.y, (int32)size.x, (int32)size.y);
                             });
        }

        //-------------------------------------------------------------------------
        void RendererAPI::begin_frame()
        {
            setup_rasterizer_state(RasterizerState());
            setup_face_cull_state(FaceCullState());
            setup_depth_test_state(DepthTestState());
        }

        //-------------------------------------------------------------------------
        void RendererAPI::begin_render_pass(ref_ptr<rex::Pipeline> pipeline, bool explicitClear /*= false*/)
        {
            g_active_renderpass = pipeline->get_render_pass();

            if (g_active_renderpass->get_frame_buffer() != nullptr)
            {
                g_active_renderpass->get_frame_buffer()->bind();
            }

            if (explicitClear)
            {
                Renderer::submit([instance = g_active_renderpass]()
                                 {
                                     auto clear_color = instance->get_clear_color();
                                     auto clear_depth = instance->get_clear_depth();

                                     opengl::clear_color(clear_color.red, clear_color.green, clear_color.blue, clear_color.alpha);
                                     opengl::clear_depth(clear_depth);

                                     auto clear_flags = instance->get_clear_flags();

                                     uint32 in_clear_flags = (uint32)clear_flags;
                                     uint32 out_clear_flags = 0;

                                     if (in_clear_flags & (uint32)ClearFlags::COLOR)
                                         out_clear_flags |= GL_COLOR_BUFFER_BIT;
                                     if (in_clear_flags & (uint32)ClearFlags::DEPTH)
                                         out_clear_flags |= GL_DEPTH_BUFFER_BIT;
                                     if (in_clear_flags & (uint32)ClearFlags::STENCIL)
                                         out_clear_flags |= GL_STENCIL_BUFFER_BIT;

                                     opengl::clear(out_clear_flags);
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void RendererAPI::end_frame()
        {
            // Nothing to implement
        }

        //-------------------------------------------------------------------------
        void RendererAPI::end_render_pass()
        {
            if (g_active_renderpass->get_frame_buffer() != nullptr)
            {
                g_active_renderpass->get_frame_buffer()->unbind();
            }

            g_active_renderpass = nullptr;
        }

        //-------------------------------------------------------------------------
        void RendererAPI::render_quad(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Material> material, const rex::matrix4& transform)
        {
            render_model_with_material(pipeline, uniformBufferSet, mesh_factory::get_unit_quad(), transform, material);
        }

        //-------------------------------------------------------------------------
        void RendererAPI::render_model(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<rex::Model> model, const rex::matrix4& transform)
        {
            setup_rasterizer_state(pipeline->get_rasterizer_state());
            setup_face_cull_state(pipeline->get_face_cull_state());
            setup_depth_test_state(pipeline->get_depth_test_state());

            model->get_vertex_buffer()->bind();
            pipeline->bind();
            if (model->get_index_buffer())
            {
                model->get_index_buffer()->bind();
            }

            Renderer::submit([pipeline, uniformBufferSet, model, transform]
                             {
                                 auto& materials = model->get_materials();
                                 for (const Submesh& submesh : model->get_submeshes())
                                 {
                                     auto material = materials[submesh.material_index];
                                     material->flush(IsRenderThread::YES);

                                     auto u_transform = transform * submesh.transform;
                                     material->get_shader_program()->set_uniform("u_Transform", u_transform, IsRenderThread::YES);

                                     if (material->has_flag(MaterialFlag::DepthTest))
                                     {
                                         opengl::enable(GL_DEPTH_TEST);
                                     }
                                     else
                                     {
                                         opengl::disable(GL_DEPTH_TEST);
                                     }

                                     if (model->get_index_buffer())
                                     {
                                         opengl::draw_elements(to_gl_primitive_type(pipeline->get_primitive_type()), submesh.index_count, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.base_index));
                                     }
                                     else
                                     {
                                         opengl::draw_arrays(to_gl_primitive_type(pipeline->get_primitive_type()), 0, submesh.vertex_count);
                                     }
                                 }
                             });

            if (model->get_index_buffer())
            {
                model->get_index_buffer()->unbind();
            }
            pipeline->unbind();
            model->get_vertex_buffer()->unbind();
        }

        //-------------------------------------------------------------------------
        void RendererAPI::render_model_with_material(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* uniformBufferSet, ref_ptr<Model> model, const rex::matrix4& transform, ref_ptr<rex::Material> material)
        {
            setup_rasterizer_state(pipeline->get_rasterizer_state());
            setup_face_cull_state(pipeline->get_face_cull_state());
            setup_depth_test_state(pipeline->get_depth_test_state());

            model->get_vertex_buffer()->bind();
            pipeline->bind();
            if (model->get_index_buffer())
            {
                model->get_index_buffer()->bind();
            }

            Renderer::submit([pipeline, uniformBufferSet, model, transform, material]() mutable
                             {
                                 for (const Submesh& submesh : model->get_submeshes())
                                 {
                                     material->flush(IsRenderThread::YES);

                                     auto u_transform = transform * submesh.transform;
                                     material->get_shader_program()->set_uniform("u_Transform", u_transform, IsRenderThread::YES);

                                     if (material->has_flag(MaterialFlag::DepthTest))
                                     {
                                         opengl::enable(GL_DEPTH_TEST);
                                     }
                                     else
                                     {
                                         opengl::disable(GL_DEPTH_TEST);
                                     }

                                     if (model->get_index_buffer())
                                     {
                                        opengl::draw_elements(to_gl_primitive_type(pipeline->get_primitive_type()), submesh.index_count, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.base_index));
                                     }
                                     else
                                     {
                                         opengl::draw_arrays(to_gl_primitive_type(pipeline->get_primitive_type()), 0, submesh.vertex_count);
                                     }
                                 }
                             });

            if (model->get_index_buffer())
            {
                model->get_index_buffer()->unbind();
            }
            pipeline->unbind();
            model->get_vertex_buffer()->unbind();
        }

        //-------------------------------------------------------------------------
        void RendererAPI::copy_framebuffer_content(uint32 fromFrameBufferID, const RectI& fromRect, uint32 toFrameBufferID, const RectI& toRect, const FrameBufferCopyOption& copyOption, const FrameBufferFilterOption& filterOption)
        {
            Renderer::submit([fromFrameBufferID, fromRect, toFrameBufferID, toRect, copyOption, filterOption]() 
                {
                    // Cache currently bounds framebuffer
                    auto current_bound_framebuffer = FrameBufferPool::instance()->get_bound();

                    opengl::bind_framebuffer(GL_READ_FRAMEBUFFER, fromFrameBufferID);
                    opengl::bind_framebuffer(GL_DRAW_FRAMEBUFFER, toFrameBufferID);

                    uint32 mask = 0;
                    switch (copyOption)
                    {
                        case FrameBufferCopyOption::COLOR: mask = GL_COLOR_BUFFER_BIT; break;
                        case FrameBufferCopyOption::DEPTH: mask = GL_DEPTH_BUFFER_BIT; break;
                        case FrameBufferCopyOption::STENCIL: mask = GL_STENCIL_BUFFER_BIT; break;
                    }

                    uint32 filter = 0;
                    switch (filterOption)
                    {
                        case FrameBufferFilterOption::LINEAR: filter = GL_LINEAR; break;
                        case FrameBufferFilterOption::NEAREST: filter = GL_NEAREST; break;
                    }

#if REX_DEBUG
                    if (copyOption == FrameBufferCopyOption::DEPTH || copyOption == FrameBufferCopyOption::STENCIL)
                    {
                        R_ASSERT_X(filterOption == FrameBufferFilterOption::NEAREST, "Invalid Operation, when blitting DEPTH or STENCIL, filter should be NEAREST");
                    }
#endif

                    opengl::blit_framebuffer(fromRect.x, fromRect.y, fromRect.width, fromRect.height, toRect.x, toRect.y, toRect.width, toRect.height, mask, filter);
                    
                    if (current_bound_framebuffer == nullptr)
                    {
                        opengl::bind_framebuffer(GL_FRAMEBUFFER, 0);
                    }
                    else
                    {
                        current_bound_framebuffer->bind(IsRenderThread::YES);
                    }
                });
        }

        //-------------------------------------------------------------------------
        void RendererAPI::submit_fullscreen_quad(ref_ptr<rex::Pipeline> pipeline, rex::UniformBufferSet* /*uniformBufferSet*/, ref_ptr<rex::Material> material)
        {
            setup_rasterizer_state(pipeline->get_rasterizer_state());
            setup_face_cull_state(pipeline->get_face_cull_state());
            setup_depth_test_state(pipeline->get_depth_test_state());

            g_fullscreen_quad_vertex_buffer->bind();
            pipeline->bind();
            g_fullscreen_quad_index_buffer->bind();

            material->flush();

            Renderer::submit([pipeline]()
                             {
                                 opengl::draw_elements(to_gl_primitive_type(pipeline->get_primitive_type()), g_fullscreen_quad_index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);
                             });

            g_fullscreen_quad_index_buffer->unbind();
            pipeline->unbind();
            g_fullscreen_quad_vertex_buffer->unbind();
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::Pipeline> RendererAPI::create_pipeline(const PipelineDescription& description) const
        {
            g_pipelines.push_back(make_ref<Pipeline>(description));

            return g_pipelines.back().get();
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::RenderPass> RendererAPI::create_render_pass(const RenderPassDescription& description) const
        {
            g_renderpasses.push_back(make_ref<RenderPass>(description));

            return g_renderpasses.back().get();
        }
    }
}