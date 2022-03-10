#pragma once

#include "types.h"

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/shaderuniformtype.h"

#include <vector>

namespace sbt
{
    class ShaderResource
    {
    public:
        enum class Domain
        {
            NONE,
            VERTEX,
            PIXEL
        };

        ShaderResource();
        ShaderResource(const StringID& name, ShaderUniformType type, uint32 count, uint32 offset, uint32 reg);

        const StringID&             getName() const;

        const ShaderUniformType&    getType() const;
        const uint32                getCount() const;
        const uint32                getOffset() const;
        const uint32                getRegister() const;

        bool                        isValid() const;

        static ShaderResource EMPTY;

    private:
        StringID m_name;
        ShaderUniformType m_type;
        uint32 m_count;
        uint32 m_offset;
        uint32 m_register;
    };

    using ShaderResources = std::vector<ShaderResource>;
}