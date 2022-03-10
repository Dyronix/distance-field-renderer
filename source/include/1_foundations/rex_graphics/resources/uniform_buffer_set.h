#pragma once

#include "memory/internalptr.h"
#include "memory/refptr.h"

#include "resources/uniformbuffer.h"
#include "resources/shaderuniformblock.h"

#include <map>

namespace sbt
{
    using UniformBufferMap = std::map<uint32_t, std::map<StringID, ref_ptr<UniformBuffer>>>;

    class UniformBufferSet : public RefCountedObject
    {
    public:
        static UniformBufferSet* instance();

        static const int32 getNewBindingPoint();
        static const int32 getBindingPointCount();

        static std::vector<int32>& getBindingPoints();

        UniformBufferSet(uint32 maxFrames = 3u);
        ~UniformBufferSet() override;

        void clear();

        ref_ptr<UniformBuffer> get(uint32 binding, uint32 frame = 0) const;
        ref_ptr<UniformBuffer> get(const StringID& name, uint32 frame = 0) const;

        StringID create(const ShaderUniformBlock& block);

    protected:
        virtual ref_ptr<UniformBuffer> createUniformBuffer(const ShaderUniformBlock& block) const = 0;

    private:
        UniformBufferMap m_uniform_buffers;

        static UniformBufferSet* s_instance;
        static std::vector<int32> s_binding_points;
    };
}