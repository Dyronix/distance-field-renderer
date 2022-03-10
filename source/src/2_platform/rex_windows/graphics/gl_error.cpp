#include "rex_windows_pch.h"

#include "graphics/gl_error.h"
#include "graphics/gl_function_library.h"
#include "graphics/gl_api.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        std::string gl_get_error_message(uint32 error)
        {
            switch (error)
            {
                case GL_INVALID_OPERATION: return "Invalid Operation";
                case GL_INVALID_ENUM: return "Invalid Enum";
                case GL_INVALID_VALUE: return "Invalid Value";
                case GL_INVALID_INDEX: return "Invalid Index";
                case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
            }

            return "Unknown error";
        }

        //-------------------------------------------------------------------------
        uint32 gl_check_error()
        {
            return opengl::get_error();
        }
        //-------------------------------------------------------------------------
        bool gl_log_call(const char* function, const char* file, int32 line)
        {
            uint32 error = gl_check_error();
            if (error == GL_NO_ERROR)
            {
                return true;
            }

            while (error != GL_NO_ERROR)
            {
                R_ERROR("[OpenGL Error] ({0}, {1}): {2} {3}: {4}", error, gl_get_error_message(error), function, file, line);

                error = gl_check_error();
            }

            return false;
        }
    }
}