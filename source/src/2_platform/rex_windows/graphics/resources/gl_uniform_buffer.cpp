#include "rex_windows_pch.h"

#include "graphics/resources/gl_uniform_buffer.h"
#include "graphics/resources/gl_uniform_buffer_set.h"

#include "renderer/renderer.h"

#include "pointer_math.h"

namespace rex
{
    namespace opengl
    {
        constexpr uint32 FRAMES_IN_FLIGHT = 1u;

        using UniformBufferManaged = YesNoEnum;

        namespace uniform_buffer_set
        {
            UniformBufferManaged g_managed(UniformBufferManaged::NO);
        }

        UniformBufferSet g_uniform_buffer_set(FRAMES_IN_FLIGHT);

        //-------------------------------------------------------------------------
        StringID UniformBuffer::create(const ShaderUniformBlock& block)
        {
            uniform_buffer_set::g_managed = UniformBufferManaged::YES;
            auto sid_name = g_uniform_buffer_set.create(block);
            uniform_buffer_set::g_managed = UniformBufferManaged::NO;

            return sid_name;
        }

        //-------------------------------------------------------------------------
        UniformBuffer::UniformBuffer(const ShaderUniformBlock& block)
            : m_buffer_id(0)
            , m_uniform_buffer_desc(block)
            , m_local_storage(memory::make_blob(nullptr, block.get_size()))
        {
            RENDERER_INFO("Submitting - Creating Uniform Buffer: name - {0}", block.get_name().to_string());

            ref_ptr<UniformBuffer> instance(this);
            Renderer::submit([instance]() mutable
                             {
                                 RENDERER_INFO("Executing - Creating Uniform Buffer: name - {0}", instance->m_uniform_buffer_desc.get_name().to_string());

                                 R_ASSERT_X(instance->m_uniform_buffer_desc.get_size() != 0, "Size of a uniform buffer cannot be 0");

                                 opengl::generate_buffers(1, &instance->m_buffer_id);
                                 opengl::bind_buffer(GL_UNIFORM_BUFFER, instance->m_buffer_id);
                                 opengl::buffer_data(GL_UNIFORM_BUFFER, instance->m_uniform_buffer_desc.get_size(), nullptr, GL_DYNAMIC_DRAW);
                                 opengl::bind_buffer(GL_UNIFORM_BUFFER, 0);

                                 opengl::bind_buffer_range(GL_UNIFORM_BUFFER, instance->m_uniform_buffer_desc.get_binding(), instance->m_buffer_id, 0, instance->m_uniform_buffer_desc.get_size());
                             });

#ifdef REX_DEBUG
            if (!uniform_buffer_set::g_managed)
            {
                R_WARN("Unmanaged uniform buffer!");
            }
#else
            R_ASSERT_X(uniform_buffer_set::g_managed, "Use \"ResourceFactory::createUniformBuffer()\" to instantiate a uniform buffer.");
#endif
        }
        //-------------------------------------------------------------------------
        UniformBuffer::~UniformBuffer()
        {
            if (m_buffer_id)
            {
                ref_ptr<UniformBuffer> instance(this);
                Renderer::submit([instance]() mutable
                                 {
                                     opengl::delete_buffers(1, &instance->m_buffer_id);
                                     instance->m_buffer_id = 0;
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void UniformBuffer::release()
        {
            if (m_buffer_id)
            {
                opengl::delete_buffers(1, &m_buffer_id);
                m_buffer_id = 0;
            }
        }

        //-------------------------------------------------------------------------
        void UniformBuffer::set_data(const void* data, uint32 size, uint32 offset /* = 0 */, IsRenderThread rt /* = IsRenderThread::NO */)
        {
            R_TODO("Fix uniform buffer size!");
            R_ASSERT(offset + size <= m_uniform_buffer_desc.get_size());

            // TODO: local storage should be potentially replaced with render thread storage
            memcpy(pointer_math::jump_forward(m_local_storage.get_data(), offset), data, size);

            if (rt)
            {
                opengl::bind_buffer(GL_UNIFORM_BUFFER, m_buffer_id);
                opengl::buffer_sub_data(GL_UNIFORM_BUFFER, offset, size, m_local_storage.get_data());
                opengl::bind_buffer(GL_UNIFORM_BUFFER, 0);
            }
            else
            {
                ref_ptr<UniformBuffer> instance(this);
                Renderer::submit([instance, offset, size]()
                                 {
                                     opengl::bind_buffer(GL_UNIFORM_BUFFER, instance->m_buffer_id);
                                     opengl::buffer_sub_data(GL_UNIFORM_BUFFER, offset, size, instance->m_local_storage.get_data());
                                     opengl::bind_buffer(GL_UNIFORM_BUFFER, 0);
                                 });
            }
        }

        //-------------------------------------------------------------------------
        rex::StringID UniformBuffer::get_name() const
        {
            return m_uniform_buffer_desc.get_name();
        }

        //-------------------------------------------------------------------------
        uint32 UniformBuffer::get_binding() const
        {
            return m_uniform_buffer_desc.get_binding();
        }
        //-------------------------------------------------------------------------
        uint32 UniformBuffer::get_size() const
        {
            return m_uniform_buffer_desc.get_size();
        }

        //-------------------------------------------------------------------------
        const rex::ShaderUniforms& UniformBuffer::get_uniforms() const
        {
            return m_uniform_buffer_desc.get_uniforms();
        }
    }
}
