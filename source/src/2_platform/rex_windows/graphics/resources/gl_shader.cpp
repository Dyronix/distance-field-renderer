#include "rex_windows_pch.h"

#include "graphics/resources/gl_shader.h"

#include "graphics/gl_function_library.h"
#include "graphics/gl_error.h"
#include "graphics/gl_api.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        std::string shader_type_to_string(const ShaderType& type)
        {
            switch (type)
            {
                case ShaderType::FRAGMENT: return "Fragment shader";
                case ShaderType::VERTEX: return "Vertex shader";
            }

            R_ASSERT_X(false, "Invalid shader type");
            return "";
        }

        //-------------------------------------------------------------------------
        uint32 compile(ShaderType type, const memory::byte* data, size_t /*dataSize*/)
        {
            uint32 shader_type = 0;
            switch (type)
            {
                case ShaderType::VERTEX: shader_type = GL_VERTEX_SHADER; break;
                case ShaderType::FRAGMENT: shader_type = GL_FRAGMENT_SHADER; break;
                default: R_ERROR("Unsupported shader type"); return 0;
            }

            // Read our shaders into the appropriate buffers
            // Create an empty vertex shader handle
            uint32 shader_id = opengl::create_shader(shader_type);

            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            // const char* source_cstr = data;
            // GLint source_length = source.size();
            opengl::shader_source(shader_id, 1, (const int8**)&data, 0);

            // Compile the vertex shader
            opengl::compile_shader(shader_id);

            int is_compiled = 0;
            opengl::get_shader_integer_value(shader_id, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE)
            {
                int max_length = 0;
                opengl::get_shader_integer_value(shader_id, GL_INFO_LOG_LENGTH, &max_length);

                // The maxLength includes the NULL character
                std::vector<int8> info_log(max_length);
                opengl::get_shader_info_log(shader_id, max_length, &max_length, &info_log[0]);

                // We don't need the shader anymore.
                opengl::delete_shader(shader_id);

                R_ERROR("{0} failed to compile due to the following issue: ", shader_type_to_string(type));
#pragma warning(push)
#pragma warning(disable : 4996)
                R_ERROR("{0}", info_log.data());
#pragma warning(pop)
                R_ASSERT_X(false, "Compiling shader - failed");

                return 0;
            }

            return shader_id;
        }

        //-------------------------------------------------------------------------
        Shader::Shader(ShaderType type, const memory::byte* data, size_t dataSize)
            : m_shader_id(0)
        {
            m_shader_id = compile(type, data, dataSize);
        }
        //-------------------------------------------------------------------------
        Shader::~Shader()
        {
            opengl::delete_shader(m_shader_id);
        }
    }
}