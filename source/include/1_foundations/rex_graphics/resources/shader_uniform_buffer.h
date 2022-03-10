#pragma once

#include "string/stringid.h"
#include "string/stringids.h"

#include "resources/shaderuniform.h"

namespace sbt
{
    struct ShaderUniformBuffer
    {
        StringID name;

        uint32 index;
        uint32 binding_point;
        uint32 size;
        uint32 buffer_id;
        
        std::vector<ShaderUniform> uniforms;
    };
}