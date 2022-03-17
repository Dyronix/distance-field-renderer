#include "rex_windows_pch.h"

#include "containers/vector_util.h"

#include "graphics/resources/gl_frame_buffer.h"
#include "graphics/resources/gl_frame_buffer_pool.h"
#include "graphics/resources/gl_render_target.h"

#include "graphics/gl_api.h"
#include "graphics/gl_function_library.h"

#include "renderer/renderer.h"

#include "algorithms/clamp.h"

namespace rex
{
    namespace opengl
    {
        using FrameBufferPoolManaged = YesNoEnum;

        //-------------------------------------------------------------------------
        void check_framebuffer_status(const StringID& name)
        {
            auto result = opengl::check_framebuffer_status(GL_FRAMEBUFFER);
            if (result == GL_FRAMEBUFFER_COMPLETE)
                return;

            switch (result)
            {
                case GL_FRAMEBUFFER_UNDEFINED:
                    R_ERROR("[OpenGL Error]: FrameBuffer {0} undefined.", name.to_string());
                    R_ASSERT_X(false, "FrameBuffer is incomplete!");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    R_ERROR("[OpenGL Error]: FrameBuffer {0} incomplete attachment.", name.to_string());
                    R_ASSERT_X(false, "FrameBuffer is incomplete!");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    R_ERROR("[OpenGL Error]: FrameBuffer {0} incomplete missing attachment.", name.to_string());
                    R_ASSERT_X(false, "FrameBuffer is incomplete!");
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    R_ERROR("[OpenGL Error]: FrameBuffer unsupported.", name.to_string());
                    R_ASSERT_X(false, "FrameBuffer is incomplete!");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    R_ERROR("[OpenGL Error]: FrameBuffer {0} incomplete multisample.", name.to_string());
                    R_ASSERT_X(false, "FrameBuffer is incomplete!");
                    break;
            }
        }

        //-------------------------------------------------------------------------
        std::vector<ref_ptr<RenderTarget>> create_color_attachments(ColorAttachmentDescriptions&& descriptions)
        {
            std::vector<ref_ptr<RenderTarget>> color_attachments;

            int32 offset = std::abs(GL_COLOR_ATTACHMENT1 - GL_COLOR_ATTACHMENT0);
            for (int32 i = 0; i < descriptions.size(); ++i)
            {
                auto color_attachment = RenderTarget::create(std::move(descriptions[i]));

                opengl::framebuffer_texture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (offset * i), GL_TEXTURE_2D, color_attachment->get_id(), 0);

                color_attachments.push_back(std::move(color_attachment));
            }

            return color_attachments;
        }
        //-------------------------------------------------------------------------
        ref_ptr<RenderTarget> create_depth_attachment(DepthAttachmentDescription&& description, FrameBufferDepthAttachmentOption option)
        {
            ref_ptr<RenderTarget> depth_attachment = nullptr;

            if (option != FrameBufferDepthAttachmentOption::NONE)
            {
                GLenum attachment_type;
                switch (option)
                {
                    case FrameBufferDepthAttachmentOption::DEPTH_ONLY: attachment_type = GL_DEPTH_ATTACHMENT; break;
                    case FrameBufferDepthAttachmentOption::STENCIL_ONLY: attachment_type = GL_STENCIL_ATTACHMENT; break;
                    case FrameBufferDepthAttachmentOption::DEPTH_STENCIL: attachment_type = GL_DEPTH_STENCIL_ATTACHMENT; break;
                    default:
                        R_ASSERT_X(false, "Unsupported Depth Attachment Option");
                        attachment_type = GL_INVALID_ENUM;
                        break;
                }

                if (attachment_type != GL_INVALID_ENUM)
                {
                    depth_attachment = RenderTarget::create(std::move(description));

                    R_ASSERT(depth_attachment != nullptr);

                    opengl::framebuffer_texture2D(GL_FRAMEBUFFER, attachment_type, GL_TEXTURE_2D, depth_attachment->get_id(), 0);
                }
            }

            return depth_attachment;
        }

        //-------------------------------------------------------------------------
        void draw_color_buffers(const std::vector<ref_ptr<RenderTarget>>& attachments)
        {
            if (!attachments.empty())
            {
                int32 offset = std::abs(GL_COLOR_ATTACHMENT1 - GL_COLOR_ATTACHMENT0);

                int32 max_attachments = 0;
                opengl::get_integer_value(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);

                R_ASSERT(max_attachments > attachments.size());

                GLenum* buffers = new GLenum[max_attachments];
                for (int32 i = 0; i < max_attachments; ++i)
                {
                    buffers[i] = GL_COLOR_ATTACHMENT0 + (offset * i);
                }

                opengl::draw_buffers(int32(attachments.size()), buffers);

                delete[] buffers;
            }
            else
            {
                // Only depth-pass
                opengl::draw_buffer(GL_NONE);
            }
        }

        namespace framebuffer_pool
        {
            FrameBufferPoolManaged g_managed(FrameBufferPoolManaged::NO);
        }

        FrameBufferPool g_frame_buffer_pool;

        //-------------------------------------------------------------------------
        ref_ptr<rex::FrameBuffer> FrameBuffer::create(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption)
        {
            framebuffer_pool::g_managed = FrameBufferPoolManaged::YES;
            auto frame_buffer = g_frame_buffer_pool.get(std::move(description), depthAttachmentOption);
            framebuffer_pool::g_managed = FrameBufferPoolManaged::NO;

            return frame_buffer;
        }

        //-------------------------------------------------------------------------
        FrameBuffer::FrameBuffer(FrameBufferDescription&& description, FrameBufferDepthAttachmentOption depthAttachmentOption)
            : m_buffer_id(0)
            , m_depth_attachment_option(depthAttachmentOption)
            , m_is_bound(false)
        {
            RENDERER_INFO("Submitting - Creating Framebuffer: {0}", description.name.to_string());

            int32 max_attachments = 0;
            opengl::get_integer_value(GL_MAX_COLOR_ATTACHMENTS, &max_attachments);

            R_ASSERT(max_attachments > description.color_attachments.size());

            m_name = description.name;
            m_width = description.width;
            m_height = description.height;

            R_ASSERT(m_width != 0);
            R_ASSERT(m_height != 0);

            invalidate(std::move(description.color_attachments), std::move(description.depth_attachment), depthAttachmentOption);

#ifdef REX_DEBUG
            if (!framebuffer_pool::g_managed)
            {
                R_WARN("Unmanaged framebuffer, adding to pool ( this will only happen in development builds!) ");
                g_frame_buffer_pool.add(this);
            }
#else
            R_ASSERT_X(framebuffer_pool::g_managed, "Use \"ResourceFactory::create_frame_buffer()\" to instantiate a framebuffer.");
#endif
        }
        //-------------------------------------------------------------------------
        FrameBuffer::~FrameBuffer()
        {
            RENDERER_INFO("Submitting - Destroying Framebuffer: {0}", m_name.to_string());

            if (m_buffer_id)
            {
                m_is_bound = false;

                ref_ptr<FrameBuffer> instance(this);
                Renderer::submit([instance]() mutable
                                 {
                                     RENDERER_INFO("Executing - Destroying Framebuffer: {0}", instance->m_name.to_string());

                                     opengl::delete_framebuffers(1, &instance->m_buffer_id);
                                     instance->m_buffer_id = 0;
                                 });
            }
        }

        //-------------------------------------------------------------------------
        rex::StringID FrameBuffer::get_name() const
        {
            return m_name;
        }
        //-------------------------------------------------------------------------
        int32 FrameBuffer::get_width() const
        {
            return m_width;
        }
        //-------------------------------------------------------------------------
        int32 FrameBuffer::get_height() const
        {
            return m_height;
        }

        //-------------------------------------------------------------------------
        const std::vector<ref_ptr<Texture>> FrameBuffer::get_color_attachments() const
        {
            std::vector<ref_ptr<Texture>> color_attachments;
            std::transform(std::begin(m_color_attachments), std::end(m_color_attachments), std::back_inserter(color_attachments), [](const auto& attachment)
                           {
                               return ref_ptr<Texture>(attachment);
                           });

            return color_attachments;
        }
        //-------------------------------------------------------------------------
        const ref_ptr<Texture> FrameBuffer::get_color_attachment(int32 attachmentIndex /*= 0*/) const
        {
            if (m_color_attachments.empty())
            {
                return nullptr;
            }

            R_ASSERT(attachmentIndex < m_color_attachments.size());

            return ref_ptr<Texture>(m_color_attachments[attachmentIndex]);
        }

        //-------------------------------------------------------------------------
        const ref_ptr<Texture> FrameBuffer::get_depth_attachment() const
        {
            return ref_ptr<Texture>(m_depth_attachment);
        }

        //-------------------------------------------------------------------------
        void FrameBuffer::invalidate(ColorAttachmentDescriptions&& colorDescriptions, DepthAttachmentDescription&& depthDescription, rex::FrameBufferDepthAttachmentOption depthAttachmentOption, IsRenderThread rt)
        {
            if (rt)
            {
                RENDERER_INFO("Executing - Creating Framebuffer: {0}", m_name.to_string());

                if (m_buffer_id)
                {
                    release();
                }

                opengl::generate_framebuffers(1, &m_buffer_id);
                opengl::bind_framebuffer(GL_FRAMEBUFFER, m_buffer_id);

                m_color_attachments = create_color_attachments(std::move(colorDescriptions));

                m_depth_attachment_option = depthAttachmentOption;
                m_depth_attachment = create_depth_attachment(std::move(depthDescription), depthAttachmentOption);

                draw_color_buffers(m_color_attachments);
                check_framebuffer_status(m_name);

                opengl::bind_framebuffer(GL_FRAMEBUFFER, 0);
            }
            else
            {
                ref_ptr<FrameBuffer> instance(this);
                Renderer::submit([ instance, color_descriptions = std::move(colorDescriptions), depth_description = std::move(depthDescription), depthAttachmentOption ]() mutable
                                 {
                                     RENDERER_INFO("Executing - Creating Framebuffer: {0}", instance->m_name.to_string());

                                     if (instance->m_buffer_id)
                                     {
                                         instance->release();
                                     }

                                     opengl::generate_framebuffers(1, &instance->m_buffer_id);
                                     opengl::bind_framebuffer(GL_FRAMEBUFFER, instance->m_buffer_id);

                                     instance->m_color_attachments = create_color_attachments(std::move(color_descriptions));

                                     instance->m_depth_attachment_option = depthAttachmentOption;
                                     instance->m_depth_attachment = create_depth_attachment(std::move(depth_description), depthAttachmentOption);

                                     draw_color_buffers(instance->m_color_attachments);
                                     check_framebuffer_status(instance->m_name);

                                     opengl::bind_framebuffer(GL_FRAMEBUFFER, 0);
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void FrameBuffer::resize(int32 width, int32 height, IsRenderThread rt)
        {
            if (m_width == width && m_height == height)
            {
                return;
            }

            R_TODO("Min/Max viewport width should be stored somewhere else and retrieved here.");
            constexpr int32 min_width = 1;
            constexpr int32 max_width = (int32)std::numeric_limits<int16>().max();
            if (width < min_width || width > max_width)
            {
                R_ERROR("Invalid width given: {0}, clamping value between [{1};{2}]", width, min_width, max_width);
                width = rex::clamp(width, min_width, max_width);
            }
            R_TODO("Min/Max viewport height should be stored somewhere else and retrieved here.");
            constexpr int32 min_height = 1;
            constexpr int32 max_height = (int32)std::numeric_limits<int16>().max();
            if (height < min_height || height > max_height)
            {
                R_ERROR("Invalid height given: {0}, clamping value between [{1};{2}]", height, min_height, max_height);
                height = rex::clamp(height, min_height, max_height);
            }

            m_width = width;
            m_height = height;

            ColorAttachmentDescriptions color_descriptions;
            for (int32 i = 0; i < gsl::narrow<int32>(m_color_attachments.size()); ++i)
            {
                auto description = m_color_attachments[i]->get_description();

                Texture2DDescription new_description;

                new_description.filters = description.filters;
                new_description.format = description.format;
                new_description.height = m_height;
                new_description.name = description.name;
                new_description.texel_format = description.texel_format;
                new_description.usage = description.usage;
                new_description.width = m_width;
                new_description.wraps = description.wraps;
                new_description.data = nullptr;

                color_descriptions.push_back(std::move(new_description));
            }

            DepthAttachmentDescription depth_description;
            if (m_depth_attachment)
            {
                auto description = m_depth_attachment->get_description();

                depth_description.filters = description.filters;
                depth_description.format = description.format;
                depth_description.height = m_height;
                depth_description.name = description.name;
                depth_description.texel_format = description.texel_format;
                depth_description.usage = description.usage;
                depth_description.width = m_width;
                depth_description.wraps = description.wraps;
                depth_description.data = nullptr;
            }

            invalidate(std::move(color_descriptions), std::move(depth_description), m_depth_attachment_option, rt);
        }

        //-------------------------------------------------------------------------
        void FrameBuffer::release()
        {
            if (m_buffer_id)
            {
                opengl::delete_framebuffers(1, &m_buffer_id);

                for (auto& color_attachment : m_color_attachments)
                {
                    color_attachment->release();
                }

                m_color_attachments.clear();

                if (m_depth_attachment)
                {
                    m_depth_attachment->release();
                    m_depth_attachment.reset();
                }

                m_buffer_id = 0;
            }
        }

        //-------------------------------------------------------------------------
        void FrameBuffer::bind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                RENDERER_INFO("Bind Framebuffer: {0}", m_name.to_string());

                opengl::bind_framebuffer(GL_FRAMEBUFFER, m_buffer_id);
                opengl::viewport(0, 0, get_width(), get_height());

                m_is_bound = true;
            }
            else
            {
                RENDERER_INFO("Submitting - Bind Framebuffer: {0}", m_name.to_string());

                ref_ptr<const FrameBuffer> instance(this);
                Renderer::submit([instance]()
                                 {
                                     RENDERER_INFO("Executing - Bind Framebuffer: {0}", instance->m_name.to_string());

                                     opengl::bind_framebuffer(GL_FRAMEBUFFER, instance->m_buffer_id);
                                     opengl::viewport(0, 0, instance->get_width(), instance->get_height());

                                     instance->m_is_bound = true;
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void FrameBuffer::unbind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                RENDERER_INFO("Unbind Framebuffer: {0}", m_name.to_string());

                opengl::bind_framebuffer(GL_FRAMEBUFFER, 0);

                m_is_bound = false;
            }
            else
            {
                RENDERER_INFO("Submitting - Unbind Framebuffer: {0}", m_name.to_string());

                ref_ptr<const FrameBuffer> instance(this);
                Renderer::submit([instance]() 
                                 {
                                     RENDERER_INFO("Executing - Unbind Framebuffer: {0}", instance->m_name.to_string());

                                     opengl::bind_framebuffer(GL_FRAMEBUFFER, 0);

                                     instance->m_is_bound = false;
                                 });
            }
        }

        //-------------------------------------------------------------------------
        bool FrameBuffer::is_bound() const
        {
            return m_is_bound;
        }

        //-------------------------------------------------------------------------
        uint32 FrameBuffer::get_id() const
        {
            return m_buffer_id;
        }

    } // namespace opengl
}