#pragma once

#pragma warning(push)
#pragma warning(disable : 4505)

namespace rex
{
    namespace opengl
    {
        std::string gl_get_error_message(uint32 error);
        uint32 gl_check_error();
        bool gl_log_call(const char* function, const char* file, int32 line);
    }
}

#ifdef REX_DEBUG
#define GL_CALL(x)                                                                                                                                                                                                                                       \
    rex::opengl::gl_check_error();                                                                                                                                                                                                                       \
    x;                                                                                                                                                                                                                                                   \
    if (!rex::opengl::gl_log_call(#x, __FILE__, __LINE__))                                                                                                                                                                                               \
    {                                                                                                                                                                                                                                                    \
        DEBUG_BREAK();                                                                                                                                                                                                                                   \
    }
#else
#define GL_CALL(x) x
#endif

#pragma warning(pop)