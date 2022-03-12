#pragma once

#include "resources/shader_uniform.h"

namespace rex
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