#include "rex_windows_pch.h"

#include "win32_context.h"

#include "graphics/gl_api.h"
#include "graphics/gl_function_library.h"

#include <SDL.h>

namespace rex
{
    namespace win32
    {
        namespace opengl
        {
            //-------------------------------------------------------------------------
            Context::Context(SDL_Window* window, const SwapInterval& interval)
                : m_sdl_window(window)
                , m_sdl_context(nullptr)
            {
                m_sdl_context = SDL_GL_CreateContext(m_sdl_window);
                if (m_sdl_context == NULL)
                {
                    R_ERROR("Could not create OpenGL context.");
                    R_ERROR(SDL_GetError());

                    return;
                }

                uint32 status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
                if (!status)
                {
                    R_ERROR("Failed to initialized GLAD with status: {0}", status);
                    return;
                }

                set_vertical_update(interval);
            }
            //-------------------------------------------------------------------------
            Context::~Context()
            {
                SDL_GL_DeleteContext(m_sdl_context);
            }

            //-------------------------------------------------------------------------
            void* Context::get_sdl_context() const
            {
                return m_sdl_context;
            }

            //-------------------------------------------------------------------------
            const char* Context::get_version() const
            {
                R_TODO("Include OpenGL version number");

                // int version_major;
                // int version_minor;

                // opengl::get_integer_value(GL_MAJOR_VERSION, &version_major);
                // opengl::get_integer_value(GL_MINOR_VERSION, &version_minor);

                return opengl::get_string_value(GL_VERSION);
            }

            //-------------------------------------------------------------------------
            const char* Context::get_renderer() const
            {
                return opengl::get_string_value(GL_RENDERER);
            }

            //-------------------------------------------------------------------------
            const char* Context::get_vendor() const
            {
                return opengl::get_string_value(GL_VENDOR);
            }

            //-------------------------------------------------------------------------
            void Context::set_vertical_update(const SwapInterval& interval)
            {
                if (SDL_GL_SetSwapInterval(gsl::narrow<int32>(interval)) == -1)
                {
                    R_ERROR("Swap interval not supported");
                    R_ERROR(SDL_GetError());

                    return;
                }
            }

            //-------------------------------------------------------------------------
            void Context::make_current()
            {
                if (SDL_GL_MakeCurrent(m_sdl_window, m_sdl_context) != 0)
                {
                    R_ERROR("Failed to make the OpenGL context the current one!");
                    R_ERROR(SDL_GetError());
                }
            }
            //-------------------------------------------------------------------------
            void Context::present()
            {
                SDL_GL_SwapWindow(m_sdl_window);
            }
        }
    }
}