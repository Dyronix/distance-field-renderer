#include "rex_windows_pch.h"

#include "graphics/renderer/gl_render_pipeline.h"
#include "graphics/renderer/gl_render_pass.h"

#include "graphics/resources/gl_shader_program.h"

#include "renderer/renderer.h"

namespace rex
{
    namespace opengl
    {
        namespace pipeline
        {
            //-------------------------------------------------------------------------
            DataType get_element_buffer_type(const DataType& type)
            {
                switch ((DataType::Type)type)
                {
                    case DataType::IVEC2:
                    case DataType::IVEC3:
                    case DataType::IVEC4:
                        return DataType::INT32;
                    case DataType::UIVEC2:
                    case DataType::UIVEC3:
                    case DataType::UIVEC4:
                        return DataType::UNSIGNED_INT32;
                    case DataType::VEC2:
                    case DataType::VEC3:
                    case DataType::VEC4:
                        return DataType::FLOAT32;
                default:
                    R_ASSERT_X(false, "Unknown element type");
                    return DataType::NONE;
                }
            }

            //-------------------------------------------------------------------------
            void enable_vertex_attribute_array(uint32 index)
            {
                opengl::enable_vertex_attrib_array(index);
            }

            //-------------------------------------------------------------------------
            void define_attribute_data_int(uint32 index, const BufferElement& element, uint32 stride)
            {
                opengl::vertex_attrib_i_pointer(
                    index,                                                      // Specifies the index of the generic vertex attribute to be modified.
                    element.component_count,                                    // Specifies the number of components per generic vertex attribute.
                    to_open_gl_data_type(get_element_buffer_type(element.type)),    // Specifies the data type of each component in the array.
                    stride,                                                     // Specifies the byte offset between consecutive generic vertex attributes. 
#pragma warning( push )
#pragma warning( disable : 4312 )
                    (const void*)element.offset);                               // Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
#pragma warning( pop )
            }
            //-------------------------------------------------------------------------
            void define_attribute_data_float(uint32 index, const BufferElement& element, uint32 stride)
            {
                opengl::vertex_attrib_pointer(
                    index,                                                      // Specifies the index of the generic vertex attribute to be modified.
                    element.component_count,                                    // Specifies the number of components per generic vertex attribute.
                    to_open_gl_data_type(get_element_buffer_type(element.type)),    // Specifies the data type of each component in the array.
                    element.normalized ? GL_TRUE : GL_FALSE,                    // Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
                    stride,                                                     // Specifies the byte offset between consecutive generic vertex attributes. 
#pragma warning( push )
#pragma warning( disable : 4312 )
                    (const void*)element.offset);                               // Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
#pragma warning( pop )
            }

            //-------------------------------------------------------------------------
            void define_vertex_attribute_data(uint32 index, const BufferElement& element, uint32 stride)
            {
                switch (element.type)
                {
                case DataType::INT32:
                case DataType::IVEC2:
                case DataType::IVEC3:
                case DataType::IVEC4:
                case DataType::UNSIGNED_INT32:
                case DataType::UIVEC2:
                case DataType::UIVEC3:
                case DataType::UIVEC4:
                case DataType::BOOL:
                    define_attribute_data_int(index, element, stride);
                    break;

                case DataType::FLOAT32:
                case DataType::VEC2:
                case DataType::VEC3:
                case DataType::VEC4:
                case DataType::MAT3:
                case DataType::MAT4:
                    define_attribute_data_float(index, element, stride);
                    break;

                default:
                    R_ASSERT_X(false, "Unsupported data type");
                    break;
                }
            }
        }

        //-------------------------------------------------------------------------
        Pipeline::Pipeline(const PipelineDescription& pipelineDesc)
            :m_description(pipelineDesc)
            ,m_vertex_array_id(0)
        {
            RENDERER_INFO("Submitting - Creating Pipeline: {0}", pipelineDesc.name.to_string());

            if (m_description.shader == nullptr)
            {
                R_ERROR("Shader not found for pipeline {0}", m_description.name.to_string());
                R_ASSERT(m_description.shader != nullptr);
            }
            if (m_description.renderpass == nullptr)
            {
                R_ERROR("Render pass not found for pipeline {0}", m_description.name.to_string());
                R_ASSERT(m_description.renderpass != nullptr);
            }

            invalidate();
        }
        //-------------------------------------------------------------------------
        Pipeline::~Pipeline()
        {
            RENDERER_INFO("Submitting - Destroying Pipeline: {0}", m_description.name.to_string());

            if (m_vertex_array_id)
            {
                ref_ptr<Pipeline> instance(this);
                Renderer::submit([instance]() mutable
                    {
                        RENDERER_INFO("Executing - Destroying Pipeline: {0}", instance->m_description.name.to_string());

                        opengl::delete_vertex_arrays(1, &instance->m_vertex_array_id);
                        instance->m_vertex_array_id = 0;
                    });
            }

            m_description.renderpass.reset();
            m_description.shader.reset();
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::ShaderProgram>& Pipeline::get_shader_program()
        {
            return m_description.shader;
        }

        //-------------------------------------------------------------------------
        const ref_ptr<rex::ShaderProgram>& Pipeline::get_shader_program() const
        {
            return m_description.shader;
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::RenderPass>& Pipeline::get_render_pass()
        {
            return m_description.renderpass;
        }

        //-------------------------------------------------------------------------
        const ref_ptr<rex::RenderPass>& Pipeline::get_render_pass() const
        {
            return m_description.renderpass;
        }

        //-------------------------------------------------------------------------
        const rex::BufferLayout& Pipeline::get_buffer_layout() const
        {
            return m_description.layout;
        }

        //-------------------------------------------------------------------------
        const rex::PrimitiveType& Pipeline::get_primitive_type() const
        {
            return m_description.primitive_type;
        }
        //-------------------------------------------------------------------------
        const rex::FaceCullState& Pipeline::get_face_cull_state() const
        {
            return m_description.facecull_state;
        }
        //-------------------------------------------------------------------------
        const rex::DepthTestState& Pipeline::get_depth_test_state() const
        {
            return m_description.depth_test_state;
        }
        //-------------------------------------------------------------------------
        const rex::RasterizerState& Pipeline::get_rasterizer_state() const
        {
            return m_description.rasterizer_state;
        }

        //-------------------------------------------------------------------------
        const rex::StringID& Pipeline::get_debug_name() const
        {
            return m_description.name;
        }

        //-------------------------------------------------------------------------
        void Pipeline::invalidate()
        {
            R_ASSERT_X(get_buffer_layout().get_elements().size(), "Layout is empty!");

            ref_ptr<Pipeline> instance(this);
            Renderer::submit([instance]() mutable
                {
                    RENDERER_INFO("Executing - Creating Pipeline: {0}", instance->m_description.name.to_string());

                    if (instance->m_vertex_array_id)
                    {
                        instance->release();
                    }

                    opengl::generate_vertex_arrays(1, &instance->m_vertex_array_id);
                });
        }

        //-------------------------------------------------------------------------
        void Pipeline::release()
        {
            if (m_vertex_array_id)
            {
                opengl::delete_vertex_arrays(1, &m_vertex_array_id);
                m_vertex_array_id = 0;
            }
        }

        //-------------------------------------------------------------------------
        void Pipeline::bind() const
        {
            RENDERER_INFO("Submitting - Bind Pipeline: {0}", m_description.name.to_string());

            ref_ptr<const Pipeline> instance(this);
            Renderer::submit([instance]()
                {
                    RENDERER_INFO("Executing - Bind Pipeline: {0}", instance->m_description.name.to_string());

                    opengl::bind_vertex_array(instance->m_vertex_array_id);

                    uint32_t attrib_index = 0;

                    for (const auto& element : instance->m_description.layout.get_elements())
                    {
                        pipeline::enable_vertex_attribute_array(attrib_index);
                        pipeline::define_vertex_attribute_data(attrib_index, element, instance->m_description.layout.get_stride());

                        attrib_index++;
                    }
                });
        }

        //-------------------------------------------------------------------------
        void Pipeline::unbind() const
        {
            RENDERER_INFO("Submitting - UnBind Pipeline: {0}", m_description.name.to_string());

            ref_ptr<const Pipeline> instance(this);
            Renderer::submit([instance]()
                {
                    RENDERER_INFO("Executing - UnBind Pipeline: {0}", instance->m_description.name.to_string());

                    opengl::bind_vertex_array(0);
                });
        }
    }
}