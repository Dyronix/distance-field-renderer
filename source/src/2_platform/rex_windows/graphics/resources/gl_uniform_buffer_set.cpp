#include "rex_windows_pch.h"

#include "graphics/resources/gl_uniform_buffer_set.h"
#include "graphics/resources/gl_uniform_buffer.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        UniformBufferSet::UniformBufferSet(uint32 maxFrames /*= 3u*/)
            : rex::UniformBufferSet(maxFrames)
        {
        }

        //-------------------------------------------------------------------------
        UniformBufferSet::~UniformBufferSet()
        {
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::UniformBuffer> UniformBufferSet::create_uniform_buffer(const ShaderUniformBlock& block) const
        {
            return ref_ptr<rex::UniformBuffer>(make_ref<rex::opengl::UniformBuffer>(block));
        }
    }
}