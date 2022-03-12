#pragma once

#include "resources/uniform_buffer.h"
#include "resources/shader_uniform_block.h"

#include <map>

namespace rex
{
    using UniformBufferMap = std::map<uint32_t, std::map<StringID, ref_ptr<UniformBuffer>>>;

    class UniformBufferSet : public RefCountedObject
    {
    public:
        static UniformBufferSet* instance();

        static const int32 get_new_binding_point();
        static const int32 get_binding_point_count();

        static std::vector<int32>& get_binding_points();

        UniformBufferSet(uint32 maxFrames = 3u);
        ~UniformBufferSet() override;

        void clear();

        ref_ptr<UniformBuffer> get(uint32 binding, uint32 frame = 0) const;
        ref_ptr<UniformBuffer> get(const StringID& name, uint32 frame = 0) const;

        StringID create(const ShaderUniformBlock& block);

    protected:
        virtual ref_ptr<UniformBuffer> create_uniform_buffer(const ShaderUniformBlock& block) const = 0;

    private:
        UniformBufferMap m_uniform_buffers;

        static UniformBufferSet* s_instance;
        static std::vector<int32> s_binding_points;
    };
}