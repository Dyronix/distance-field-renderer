#include "rex_windows_pch.h"

#include "graphics/gl_api.h"

#include "graphics/renderer/gl_renderer_api.h"
#include "graphics/resources/gl_resources_api.h"

namespace rex
{
    //-------------------------------------------------------------------------
    std::unique_ptr<ResourcesAPI> create_resources_api()
    {
        return std::make_unique<opengl::ResourcesAPI>();
    }
    //-------------------------------------------------------------------------
    std::unique_ptr<RendererAPI> create_renderer_api()
    {
        return std::make_unique<opengl::RendererAPI>();
    }

    //-------------------------------------------------------------------------
    GLenum to_open_gl_data_type(DataType type)
    {
        switch (type)
        {
            case DataType::INT32: return GL_INT;
            case DataType::UNSIGNED_INT32: return GL_UNSIGNED_INT;
            case DataType::FLOAT32: return GL_FLOAT;

            case DataType::MAT3: return GL_FLOAT_MAT3;
            case DataType::MAT4: return GL_FLOAT_MAT4;

            case DataType::VEC2: return GL_FLOAT_VEC2;
            case DataType::VEC3: return GL_FLOAT_VEC3;
            case DataType::VEC4: return GL_FLOAT_VEC4;

            case DataType::IVEC2: return GL_INT;
            case DataType::IVEC3: return GL_INT;
            case DataType::IVEC4: return GL_INT;

            case DataType::UIVEC2: return GL_UNSIGNED_INT;
            case DataType::UIVEC3: return GL_UNSIGNED_INT;
            case DataType::UIVEC4: return GL_UNSIGNED_INT;

            case DataType::BOOL: return GL_BOOL;

            case DataType::SAMPLER2D: return GL_SAMPLER_2D;
            case DataType::SAMPLER3D: return GL_SAMPLER_3D;
            case DataType::SAMPLERCUBE: return GL_SAMPLER_CUBE;

            default: R_ASSERT_X(false, "Invalid DataType"); return gsl::narrow<GLenum>(-1);
        }
    }

    //-------------------------------------------------------------------------
    rex::DataType from_open_gl_data_type(GLenum type)
    {
        switch (type)
        {
            case GL_INT: return ShaderUniformType::INT32;
            case GL_UNSIGNED_INT: return ShaderUniformType::UNSIGNED_INT32;
            case GL_FLOAT: return ShaderUniformType::FLOAT32;

            case GL_FLOAT_MAT3: return ShaderUniformType::MAT3;
            case GL_FLOAT_MAT4: return ShaderUniformType::MAT4;

            case GL_FLOAT_VEC2: return ShaderUniformType::VEC2;
            case GL_FLOAT_VEC3: return ShaderUniformType::VEC3;
            case GL_FLOAT_VEC4: return ShaderUniformType::VEC4;

            case GL_INT_VEC2: return ShaderUniformType::IVEC2;
            case GL_INT_VEC3: return ShaderUniformType::IVEC3;
            case GL_INT_VEC4: return ShaderUniformType::IVEC4;

            case GL_UNSIGNED_INT_VEC2: return ShaderUniformType::UIVEC2;
            case GL_UNSIGNED_INT_VEC3: return ShaderUniformType::UIVEC3;
            case GL_UNSIGNED_INT_VEC4: return ShaderUniformType::UIVEC4;

            case GL_BOOL: return ShaderUniformType::BOOL;

            case GL_SAMPLER_2D: return ShaderUniformType::SAMPLER2D;
            case GL_SAMPLER_3D: return ShaderUniformType::SAMPLER3D;
            case GL_SAMPLER_CUBE: return ShaderUniformType::SAMPLERCUBE;

            default: R_ASSERT_X(false, "Unsupported uniform type"); return ShaderUniformType::NONE;
        }
    }
}