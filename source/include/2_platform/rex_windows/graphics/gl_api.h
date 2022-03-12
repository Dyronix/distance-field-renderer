#pragma once

#if defined REX_PLATFORM_WINDOWS
#include <glad/glad.h>
#else
#error Unsupported platform
#endif

#include "data_type.h"

namespace rex
{
    using IsRenderThread = YesNoEnum;

    class ResourcesAPI;
    class RendererAPI;

    std::unique_ptr<ResourcesAPI> create_resources_api();
    std::unique_ptr<RendererAPI> create_renderer_api();

    GLenum to_open_gl_data_type(DataType type);
    DataType from_open_gl_data_type(GLenum type);
}