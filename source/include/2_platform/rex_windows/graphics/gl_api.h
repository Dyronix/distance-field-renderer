#pragma once

#if defined REX_PLATFORM_WINDOWS
#include <glad/glad.h>
#else
#error Unsupported platform
#endif

namespace rex
{
    using IsRenderThread = YesNoEnum;
}