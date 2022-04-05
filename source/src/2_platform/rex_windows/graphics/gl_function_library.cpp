#include "rex_windows_pch.h"

#include "graphics/gl_function_library.h"
#include "graphics/gl_error.h"
#include "graphics/gl_api.h"

#define ENABLE_GL_FUNCTION_PROFILE 0
#define ENABLE_GL_FUNCTION_LOGGING 0
#define ENABLE_GL_GET_ERROR_LOGGING 0
#define ENABLE_GL_PARAMETER_LOGGING 1

#if ENABLE_GL_FUNCTION_LOGGING
#define GL_LOG R_INFO
#else
#define GL_LOG(...) UNUSED_PARAM(__VA_ARGS__)
#endif

#if ENABLE_GL_FUNCTION_PROFILE
    #define GL_FUNC_PROFILE R_PROFILE_FUNCTION
#else
    #define GL_FUNC_PROFILE void
#endif

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        std::string pixeltype_to_string(const uint32 pixelType)
        {
            switch (pixelType)
            {
                case GL_UNSIGNED_BYTE: return "GL_UNSIGNED_BYTE";
                case GL_BYTE: return "GL_BYTE";

                case GL_UNSIGNED_SHORT: return "GL_UNSIGNED_SHORT";
                case GL_SHORT: return "GL_SHORT";

                case GL_INT: return "GL_INT";
                case GL_UNSIGNED_INT: return "GL_UNSIGNED_INT";

                case GL_HALF_FLOAT: return "GL_HALF_FLOAT";
                case GL_FLOAT: return "GL_FLOAT";

                case GL_UNSIGNED_INT_24_8: return "GL_UNSIGNED_INT_24_8";
                case GL_FLOAT_32_UNSIGNED_INT_24_8_REV: return "GL_FLOAT_32_UNSIGNED_INT_24_8_REV";
            }

            R_WARN("Unknown pxiel type: {0}", pixelType);
            return std::to_string(pixelType);
        }

        //-------------------------------------------------------------------------
        std::string format_to_string(const uint32 format)
        {
            switch (format)
            {
                case GL_RED: return "GL_RED";
                case GL_RG: return "GL_RG";
                case GL_RGB: return "GL_RGB";
                case GL_RGBA: return "GL_RGBA";

                case GL_DEPTH_COMPONENT: return "GL_DEPTH_COMPONENT";
                case GL_DEPTH_STENCIL: return "GL_DEPTH_STENCIL";
            }

            R_WARN("Unknown format: {0}", format);
            return std::to_string(format);
        }

        //-------------------------------------------------------------------------
        std::string internal_format_to_string(const uint32 format)
        {
            switch (format)
            {
                case GL_R8: return "GL_R8";
                case GL_R8I: return "GL_R8I";
                case GL_R8UI: return "GL_R8UI";
                case GL_R16: return "GL_R16";

                case GL_R16I: return "GL_R16I";
                case GL_R16UI: return "GL_R16UI";
                case GL_R16F: return "GL_R16F";
                case GL_R32I: return "GL_R32I";

                case GL_R32UI: return "GL_R32UI";
                case GL_R32F: return "GL_R32F";
                case GL_RG8: return "GL_RG8";

                case GL_RG8I: return "GL_RG8I";
                case GL_RG8UI: return "GL_RG8UI";

                case GL_RG16: return "GL_RG16";
                case GL_RG16I: return "GL_RG16I";
                case GL_RG16UI: return "GL_RG16UI";
                case GL_RG16F: return "GL_RG16F";
                case GL_RG32I: return "GL_RG32I";
                case GL_RG32UI: return "GL_RG32UI";
                case GL_RG32F: return "GL_RG32F";
                case GL_RGB8: return "GL_RGB8";

                case GL_RGB8I: return "GL_RGB8I";
                case GL_RGB8UI: return "GL_RGB8UI";

                case GL_RGB16: return "GL_RGB16";
                case GL_RGB16I: return "GL_RGB16I";
                case GL_RGB16UI: return "GL_RGB16UI";
                case GL_RGB16F: return "GL_RGB16F";

                case GL_RGB32I: return "GL_RGB32I";
                case GL_RGB32UI: return "GL_RGB32UI";
                case GL_RGB32F: return "GL_RGB32F";

                case GL_RGBA8: return "GL_RGBA8";
                case GL_RGBA8I: return "GL_RGBA8I";
                case GL_RGBA8UI: return "GL_RGBA8UI";

                case GL_RGBA16: return "GL_RGBA16";
                case GL_RGBA16I: return "GL_RGBA16I";
                case GL_RGBA16UI: return "GL_RGBA16UI";
                case GL_RGBA16F: return "GL_RGBA16F";

                case GL_RGBA32I: return "GL_RGBA32I";
                case GL_RGBA32UI: return "GL_RGBA32UI";
                case GL_RGBA32F: return "GL_RGBA32F";
                case GL_DEPTH_COMPONENT16: return "GL_DEPTH_COMPONENT16";
                case GL_DEPTH_COMPONENT24: return "GL_DEPTH_COMPONENT24";

                case GL_DEPTH_COMPONENT32: return "GL_DEPTH_COMPONENT32";
                case GL_DEPTH_COMPONENT32F: return "GL_DEPTH_COMPONENT32F";
                case GL_DEPTH24_STENCIL8: return "GL_DEPTH24_STENCIL8";
                case GL_DEPTH32F_STENCIL8: return "GL_DEPTH32F_STENCIL8";
            }

            R_WARN("Unknown internal format: {0}", format);
            return std::to_string(format);
        }

        //-------------------------------------------------------------------------
        std::string buffer_target_to_string(const uint32 target)
        {
            switch (target)
            {
                case GL_ARRAY_BUFFER: return "Vertex attributes";
                case GL_ATOMIC_COUNTER_BUFFER: return "Atomic counter storage";
                case GL_COPY_READ_BUFFER: return "Buffer copy source";
                case GL_COPY_WRITE_BUFFER: return "Buffer copy destination";
                case GL_DISPATCH_INDIRECT_BUFFER: return "Indirect compute dispatch commands";
                case GL_DRAW_INDIRECT_BUFFER: return "Indirect command arguments";
                case GL_ELEMENT_ARRAY_BUFFER: return "Vertex array indices";
                case GL_PIXEL_PACK_BUFFER: return "Pixel read target";
                case GL_PIXEL_UNPACK_BUFFER: return "Texture data source";
                case GL_QUERY_BUFFER: return "Query result buffer";
                case GL_SHADER_STORAGE_BUFFER: return "Read-write storage for shaders";
                case GL_TEXTURE_BUFFER: return "Texture data buffer";
                case GL_TRANSFORM_FEEDBACK_BUFFER: return "Transform feedback buffer";
                case GL_UNIFORM_BUFFER: return "Uniform block storage";
            }

            R_WARN("Unknown buffer target: {0}", target);
            return std::to_string(target);
        }
        //-------------------------------------------------------------------------
        std::string texture_target_to_string(const uint32 target)
        {
            switch (target)
            {
                case GL_TEXTURE_1D: return "Texture 1D";
                case GL_TEXTURE_2D: return "Texture 2D";
                case GL_TEXTURE_3D: return "Texture 3D";
                case GL_TEXTURE_1D_ARRAY: return "Texture 1D Array";
                case GL_TEXTURE_2D_ARRAY: return "Texture 2D Array";
                case GL_TEXTURE_RECTANGLE: return "Texture Rectangle";
                case GL_TEXTURE_CUBE_MAP: return "Texture Cube Map";
                case GL_TEXTURE_CUBE_MAP_ARRAY: return "Texture Cube Map Array";
                case GL_TEXTURE_CUBE_MAP_POSITIVE_X: return "Texture Cube Map Positive X";
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_X: return "Texture Cube Map Negative X";
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Y: return "Texture Cube Map Positive Y";
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y: return "Texture Cube Map Negative Y";
                case GL_TEXTURE_CUBE_MAP_POSITIVE_Z: return "Texture Cube Map Positive Z";
                case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z: return "Texture Cube Map Negative Z";
                case GL_TEXTURE_BUFFER: return "Texture Buffer";
                case GL_TEXTURE_2D_MULTISAMPLE: return "Texture 2D Multisample";
                case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: return "Texture 2D Multisample Array";
            }

            R_WARN("Unknown texture target: {0}", target);
            return std::to_string(target);
        }
        //-------------------------------------------------------------------------
        std::string shadertype_to_string(const uint32 shaderType)
        {
            switch (shaderType)
            {
                case GL_COMPUTE_SHADER: return "Compute Shader";
                case GL_VERTEX_SHADER: return "Vertex Shader";
                case GL_TESS_CONTROL_SHADER: return "Tessalation Control Shader";
                case GL_TESS_EVALUATION_SHADER: return "Tessalation Evaluation Shader";
                case GL_GEOMETRY_SHADER: return "Geometry Shader";
                case GL_FRAGMENT_SHADER: return "Fragment Shader";
            }

            R_WARN("Unknown shader type: {0}", shaderType);
            return std::to_string(shaderType);
        }
        //-------------------------------------------------------------------------
        std::string framebuffer_target_to_string(const uint32 target)
        {
            switch (target)
            {
                case GL_DRAW_FRAMEBUFFER: return "Draw Framebuffer";
                case GL_READ_FRAMEBUFFER: return "Read Framebuffer";
                case GL_FRAMEBUFFER: return "Framebuffer";
            }

            R_WARN("Unknown framebuffer target: {0}", target);
            return std::to_string(target);
        }
        //-------------------------------------------------------------------------
        std::string drawbuffer_target_to_string(const uint32 target)
        {
            switch (target)
            {
                case GL_NONE: return "GL_NONE, No color buffers are written.";
                case GL_FRONT_LEFT: return "GL_FRONT_LEFT, Only the front left color buffer is written.";
                case GL_FRONT_RIGHT: return "GL_FRONT_RIGHT, Only the front right color buffer is written.";
                case GL_BACK_LEFT: return "GL_BACK_LEFT, Only the back left color buffer is written.";
                case GL_BACK_RIGHT: return "GL_BACK_RIGHT, Only the back right color buffer is written.";
                case GL_FRONT: return "GL_FRONT, Only the front left and front right color buffers are written.";
                case GL_BACK: return "GL_BACK, Only the back left and back right color buffers are written.";
                case GL_LEFT: return "GL_LEFT, Only the front left and back left color buffers are written.";
                case GL_RIGHT: return "GL_RIGHT, Only the front right and back right color buffers are written.";
                case GL_FRONT_AND_BACK: return "GL_FRONT_AND_BACK, All the front and back color buffers (front left, front right, back left, back right) are written.";
            }

            R_WARN("Unknown drawbuffer target: {0}", target);
            return std::to_string(target);
        }
        //-------------------------------------------------------------------------
        std::string buffer_usage_to_string(const uint32 usage)
        {
            switch (usage)
            {
                case GL_STREAM_DRAW: return "Stream Draw";
                case GL_STREAM_READ: return "Stream Read";
                case GL_STREAM_COPY: return "Stream Copy";
                case GL_STATIC_DRAW: return "Static Draw";
                case GL_STATIC_READ: return "Static Read";
                case GL_STATIC_COPY: return "Static Copy";
                case GL_DYNAMIC_DRAW: return "Dynamic Draw";
                case GL_DYNAMIC_READ: return "Dynamic Read";
                case GL_DYNAMIC_COPY: return "Dynamic Copy";
            }

            R_WARN("Unknown buffer usage: {0}", usage);
            return std::to_string(usage);
        }
        //-------------------------------------------------------------------------
        std::string cull_type_to_string(const uint32 cullType)
        {
            switch (cullType)
            {
                case GL_BACK: return "Back";
                case GL_FRONT: return "Front";
                case GL_FRONT_AND_BACK: return "Front And Back";
            }

            R_WARN("Unknown cull type: {0}", cullType);
            return std::to_string(cullType);
        }
        //-------------------------------------------------------------------------
        std::string front_face_to_string(const uint32 frontFace)
        {
            switch (frontFace)
            {
                case GL_CW: return "Clockwise";
                case GL_CCW: return "Counter Clockwise";
            }

            R_WARN("Unknown front face type: {0}", frontFace);
            return std::to_string(frontFace);
        }
        //-------------------------------------------------------------------------
        std::string depth_function_to_string(const uint32 depthFn)
        {
            switch (depthFn)
            {
                case GL_ALWAYS: return "Always";
                case GL_NEVER: return "Never";
                case GL_LESS: return "Less Then";
                case GL_LEQUAL: return "Less Then Or Equal";
                case GL_GREATER: return "Greater Then";
                case GL_GEQUAL: return "Greater Then Or Equal";
                case GL_EQUAL: return "Equal";
                case GL_NOTEQUAL: return "Not Equal";
            }

            R_WARN("Unknown depth function: {0}", depthFn);
            return std::to_string(depthFn);
        }
        //-------------------------------------------------------------------------
        std::string fill_mode_to_string(const uint32 fillMode)
        {
            switch (fillMode)
            {
                case GL_POINT: return "Point";
                case GL_LINE: return "Line";
                case GL_FILL: return "Fill";
            }

            R_WARN("Unknown fill mode: {0}", fillMode);
            return std::to_string(fillMode);
        }
        //-------------------------------------------------------------------------
        std::string enable_to_string(const uint32 enable)
        {
            switch (enable)
            {
                case GL_BLEND: return "Blending";
                case GL_CULL_FACE: return "Cull Face";
                case GL_DEPTH_TEST: return "Depth Test";
                case GL_STENCIL_TEST: return "Stencil Test";
            }

            R_WARN("Unknown state, this list is incomplete maybe you need to add another element: {0}", enable);
            return std::to_string(enable);
        }
        //-------------------------------------------------------------------------
        std::string disable_to_string(const uint32 disable)
        {
            switch (disable)
            {
                case GL_BLEND: return "Blending";
                case GL_CULL_FACE: return "Cull Face";
                case GL_DEPTH_TEST: return "Depth Test";
                case GL_STENCIL_TEST: return "Stencil Test";
            }

            R_WARN("Unknown state, this list is incomplete maybe you need to add another element: {0}", disable);
            return std::to_string(disable);
        }
        //-------------------------------------------------------------------------
        std::string blit_framebuffer_mask(const uint32 mask)
        {
            switch (mask)
            {
                case GL_COLOR_BUFFER_BIT: return "Color Buffer Bit";
                case GL_DEPTH_BUFFER_BIT: return "Depth Buffer Bit";
                case GL_STENCIL_BUFFER_BIT: return "Stencil Buffer Bit";
            }

            R_WARN("Unknown mask, this list is incomplete maybe you need to add another element: {0}", mask);
            return std::to_string(mask);
        }
        //-------------------------------------------------------------------------
        std::string blit_framebuffer_filter(const uint32 filter)
        {
            switch (filter)
            {
                case GL_NEAREST: return "Nearest";
                case GL_LINEAR: return "Linear";
            }

            R_WARN("Unknown filter, this list is incomplete maybe you need to add another element: {0}", filter);
            return std::to_string(filter);
        }

        //-------------------------------------------------------------------------
        void enable_vertex_attrib_array(uint32 index)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glEnableVertexAttribArray");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tindex: {0}", index);
#endif

            GL_CALL(glEnableVertexAttribArray(index));
        }
        //-------------------------------------------------------------------------
        void vertex_attrib_i_pointer(uint32 index, int32 size, uint32 type, size_t stride, const void* ptr)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glVertexAttribIPointer");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tindex: {0}", index);
            GL_LOG("\tsize: {0}", size);
            GL_LOG("\ttype: {0}", type);
            GL_LOG("\tstride: {0}", stride);
            GL_LOG("\tpointer: {0}", ptr);
#endif

            GL_CALL(glVertexAttribIPointer(index, size, type, gsl::narrow<GLsizei>(stride), ptr));
        }
        //-------------------------------------------------------------------------
        void vertex_attrib_pointer(uint32 index, int32 size, uint32 type, bool normalized, size_t stride, const void* ptr)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glVertexAttribPointer");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tindex: {0}", index);
            GL_LOG("\tsize: {0}", size);
            GL_LOG("\ttype: {0}", type);
            GL_LOG("\tnormalized: {0}", normalized);
            GL_LOG("\tstride: {0}", stride);
            GL_LOG("\tpointer: {0}", ptr);
#endif

            GL_CALL(glVertexAttribPointer(index, size, type, normalized, gsl::narrow<GLsizei>(stride), ptr));
        }
        //-------------------------------------------------------------------------
        void delete_vertex_arrays(size_t count, const uint32* arrays)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDeleteVertexArrays");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tarrays: {0}", fmt::ptr(arrays));
#endif

            GL_CALL(glDeleteVertexArrays(gsl::narrow<GLsizei>(count), arrays));
        }
        //-------------------------------------------------------------------------
        void generate_vertex_arrays(size_t count, uint32* arrays)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGenVertexArrays");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tarrays: {0}", fmt::ptr(arrays));
#endif

            GL_CALL(glGenVertexArrays(gsl::narrow<GLsizei>(count), arrays));
        }
        //-------------------------------------------------------------------------
        void bind_vertex_array(uint32 arrayID)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBindVertexArray");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tarray: {0}", arrayID);
#endif

            GL_CALL(glBindVertexArray(arrayID));
        }

        //-------------------------------------------------------------------------
        void viewport(int32 x, int32 y, int32 width, int32 height)
        {
            GL_FUNC_PROFILE();

            R_ASSERT_X(width > 0 && width < (int32)std::numeric_limits<uint16>().max(), "Invalid width given for viewport resize");
            R_ASSERT_X(height > 0 && height < (int32)std::numeric_limits<uint16>().max(), "Invalid height given for viewport resize");

            GL_LOG("glViewport");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
            GL_LOG("\twidth: {0}", width);
            GL_LOG("\theight: {0}", height);
#endif

            GL_CALL(glViewport(x, y, width, height));
        }

        //-------------------------------------------------------------------------
        void clear_color(float red, float green, float blue, float alpha)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glClearColor");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tred: {0}", red);
            GL_LOG("\tgreen: {0}", green);
            GL_LOG("\tblue: {0}", blue);
            GL_LOG("\talpha: {0}", alpha);
#endif

            GL_CALL(glClearColor(red, green, blue, alpha));
        }
        //-------------------------------------------------------------------------
        void clear_depth(double depth)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glClearDepth");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdepth: {0}", depth);
#endif

            GL_CALL(glClearDepth(depth));
        }
        //-------------------------------------------------------------------------
        void clear_depth_f(float depth)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glClearDepthf");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdepth: {0}", depth);
#endif

            GL_CALL(glClearDepthf(depth));
        }
        //-------------------------------------------------------------------------
        void clear(uint32 bitfield)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glClear");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tbitfield: {0}", bitfield);
#endif

            GL_CALL(glClear(bitfield));
        }

        //-------------------------------------------------------------------------
        void enable(uint32 value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glEnable");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tvalue: {0}", enable_to_string(value));
#endif

            GL_CALL(glEnable(value));
        }
        //-------------------------------------------------------------------------
        void disable(uint32 value)
        {
            GL_LOG("glDisable");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tvalue: {0}", disable_to_string(value));
#endif

            GL_CALL(glDisable(value));
        }

        //-------------------------------------------------------------------------
        void cull_face(uint32 mode)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glCullFace");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tmode: {0}", cull_type_to_string(mode));
#endif

            GL_CALL(glCullFace(mode));
        }
        //-------------------------------------------------------------------------
        void front_face(uint32 mode)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glFrontFace");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tmode: {0}", front_face_to_string(mode));
#endif

            GL_CALL(glFrontFace(mode));
        }
        //-------------------------------------------------------------------------
        void depth_func(uint32 fn)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDepthFunc");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tfunction: {0}", depth_function_to_string(fn));
#endif

            GL_CALL(glDepthFunc(fn));
        }
        //-------------------------------------------------------------------------
        void depth_mask(uint8 mode)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDepthMask");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tenabled: {0}", mode == 0 ? "FALSE" : "TRUE");
#endif

            GL_CALL(glDepthMask(mode));
        }
        //-------------------------------------------------------------------------
        void polygon_mode(uint32 face, uint32 mode)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glPolygonMode");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tface: {0}", cull_type_to_string(face));
            GL_LOG("\tmode: {0}", fill_mode_to_string(mode));
#endif

            GL_CALL(glPolygonMode(face, mode));
        }

        //-------------------------------------------------------------------------
        void draw_elements(uint32 mode, size_t count, uint32 type, const void* indices)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDrawElements");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tmode: {0}", mode);
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\ttype: {0}", type);
            GL_LOG("\tindices: {0}", fmt::ptr(indices));
#endif

            GL_CALL(glDrawElements(mode, gsl::narrow<GLsizei>(count), type, indices));
        }
        //-------------------------------------------------------------------------
        void draw_elements_base_vertex(uint32 mode, size_t count, uint32 type, const void* indices, int32 baseVertex)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDrawElementsBaseVertex");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tmode: {0}", mode);
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\ttype: {0}", type);
            GL_LOG("\tindices: {0}", fmt::ptr(indices));
            GL_LOG("\tbase vertex: {0}", baseVertex);
#endif

            GL_CALL(glDrawElementsBaseVertex(mode, gsl::narrow<GLsizei>(count), type, indices, baseVertex));
        }

        //-------------------------------------------------------------------------
        void draw_arrays(uint32 mode, int32 first, size_t count)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDrawArrays");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tmode: {0}", mode);
            GL_LOG("\tfirst: {0}", first);
            GL_LOG("\tcount: {0}", count);
#endif

            GL_CALL(glDrawArrays(mode, first, gsl::narrow<GLsizei>(count)));
        }

        //-------------------------------------------------------------------------
        void get_integer_value(uint32 pname, int32* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetIntegerv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetIntegerv(pname, data));
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdata result: {0}", *data);
#endif
        }
        //-------------------------------------------------------------------------
        void get_float_value(uint32 pname, float* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetFloatv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetFloatv(pname, data));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdata result: {0}", *data);
#endif
        }
        //-------------------------------------------------------------------------
        void get_double_value(uint32 pname, double* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetDoublev");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetDoublev(pname, data));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdata result: {0}", *data);
#endif
        }
        //-------------------------------------------------------------------------
        void get_bool_value(uint32 pname, uint8* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetBooleanv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetBooleanv(pname, data));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdata result: {0}", *data);
#endif
        }
        //-------------------------------------------------------------------------
        const char* get_string_value(uint32 name)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetString");

            const GLubyte* s;
            GL_CALL(s = glGetString(name));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tdata result: {0}", *s);
#endif

            return (const char*)s;
        }

        //-------------------------------------------------------------------------
        uint32 get_error()
        {
            GL_FUNC_PROFILE();

#if ENABLE_GL_FUNCTION_LOGGING && ENABLE_GL_GET_ERROR_LOGGING
            GL_LOG("glGetError");
#endif

            uint32 error = GL_NO_ERROR;

            error = glGetError();  // no GL_CALL here to avoid recursive function call.

#if ENABLE_GL_FUNCTION_LOGGING && ENABLE_GL_GET_ERROR_LOGGING && ENABLE_GL_PARAMETER_LOGGING
            GL_LOG("\tdata result: {0}", error);
#endif

            return error;
        }

        //-------------------------------------------------------------------------
        uint32 check_framebuffer_status(uint32 target)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glCheckFramebufferStatus");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", framebuffer_target_to_string(target));
#endif

            uint32 result = GL_FRAMEBUFFER_COMPLETE;

            GL_CALL(result = glCheckFramebufferStatus(target));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tresult: {0}", result);
#endif

            return result;
        }
        //-------------------------------------------------------------------------
        void delete_framebuffers(size_t count, const uint32* framebuffers)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDeleteFramebuffers");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tframebuffers: {0}", fmt::ptr(framebuffers));
#endif

            GL_CALL(glDeleteFramebuffers(gsl::narrow<GLsizei>(count), framebuffers));
        }
        //-------------------------------------------------------------------------
        void generate_framebuffers(size_t count, uint32* framebuffers)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGenFramebuffers");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tframebuffers: {0}", fmt::ptr(framebuffers));
#endif

            GL_CALL(glGenFramebuffers(gsl::narrow<GLsizei>(count), framebuffers));
        }
        //-------------------------------------------------------------------------
        void bind_framebuffer(uint32 target, int32 index)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBindFramebuffer");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", framebuffer_target_to_string(target));
            GL_LOG("\tindex: {0}", index);
#endif

            GL_CALL(glBindFramebuffer(target, index));
        }

        //-------------------------------------------------------------------------
        void blit_framebuffer(int32 srcx, int32 srcy, int32 srcWidth, int32 srcHeight, int32 dstx, int32 dsty, int32 dstWidth, int32 dstHeight, uint32 mask, uint32 filter)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBlitFramebuffer");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tsource x: {0}", srcx);
            GL_LOG("\tsource y: {0}", srcy);
            GL_LOG("\tsource width: {0}", srcWidth);
            GL_LOG("\tsource height: {0}", srcHeight);

            GL_LOG("\tdestination x: {0}", dstx);
            GL_LOG("\tdestination y: {0}", dsty);
            GL_LOG("\tdestination width: {0}", dstWidth);
            GL_LOG("\tdestination height: {0}", dstHeight);
            
            GL_LOG("\tmask: {0}", blit_framebuffer_mask(mask));
            GL_LOG("\tfilter: {0}", blit_framebuffer_filter(filter));
#endif
            GL_CALL(glBlitFramebuffer(srcx, srcy, srcWidth, srcHeight, dstx, dsty, dstWidth, dstHeight, mask, filter));
        }

        //-------------------------------------------------------------------------
        void framebuffer_texture2D(uint32 target, uint32 attachment, uint32 textureTarget, uint32 texture, int32 level)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glFramebufferTexture2D");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", framebuffer_target_to_string(target));
            GL_LOG("\tattachment: {0}", attachment);
            GL_LOG("\ttexture target: {0}", texture_target_to_string(textureTarget));
            GL_LOG("\ttexture: {0}", texture);
            GL_LOG("\tlevel: {0}", level);
#endif

            GL_CALL(glFramebufferTexture2D(target, attachment, textureTarget, texture, level));
        }

        //-------------------------------------------------------------------------
        void draw_buffer(uint32 target)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDrawBuffer");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", drawbuffer_target_to_string(target));
#endif

            GL_CALL(glDrawBuffer(target));
        }
        //-------------------------------------------------------------------------
        void draw_buffers(size_t count, const uint32* buffers)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDrawBuffers");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tbuffers: {0}", fmt::ptr(buffers));
#endif

            GL_CALL(glDrawBuffers(gsl::narrow<GLsizei>(count), buffers));
        }

        //-------------------------------------------------------------------------
        void get_framebuffer_integer_parameter(uint32 target, uint32 value, int32* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetFramebufferParameteriv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", target);
            GL_LOG("\tvalue: {0}", value);
#endif
            GL_CALL(glGetFramebufferParameteriv(target, value, data));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tresult data: {0}", *data);
#endif
        }

        //-------------------------------------------------------------------------
        void delete_buffers(size_t count, const uint32* buffers)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDeleteBuffers");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tbuffers: {0}", fmt::ptr(buffers));
#endif

            GL_CALL(glDeleteBuffers(gsl::narrow<GLsizei>(count), buffers));
        }
        //-------------------------------------------------------------------------
        void generate_buffers(size_t count, uint32* buffers)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGenBuffers");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tbuffers: {0}", fmt::ptr(buffers));
#endif

            GL_CALL(glGenBuffers(gsl::narrow<GLsizei>(count), buffers));
        }
        //-------------------------------------------------------------------------
        void bind_buffer(uint32 target, uint32 index)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBindBuffer");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", buffer_target_to_string(target));
            GL_LOG("\tindex: {0}", index);
#endif

            GL_CALL(glBindBuffer(target, index));
        }
        //-------------------------------------------------------------------------
        void buffer_data(uint32 target, uint32 size, const void* data, uint32 usage)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBufferData");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", buffer_target_to_string(target));
            GL_LOG("\tsize: {0}", size);
            GL_LOG("\tdata: {0}", fmt::ptr(data));
            GL_LOG("\tusage: {0}", buffer_usage_to_string(usage));
#endif

            GL_CALL(glBufferData(target, size, data, usage));
        }
        //-------------------------------------------------------------------------
        void bind_buffer_range(uint32 target, uint32 index, uint32 buffer, int64 offset, int64 size)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBindBufferRange");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", buffer_target_to_string(target));
            GL_LOG("\tindex: {0}", index);
            GL_LOG("\tbuffer: {0}", buffer);
            GL_LOG("\toffset: {0}", offset);
            GL_LOG("\tsize: {0}", size);
#endif

            GL_CALL(glBindBufferRange(target, index, buffer, offset, size));
        }
        //-------------------------------------------------------------------------
        void buffer_sub_data(uint32 target, int64 offset, int64 size, const void* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBufferSubData");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", buffer_target_to_string(target));
            GL_LOG("\toffset: {0}", offset);
            GL_LOG("\tsize: {0}", size);
            GL_LOG("\tdata: {0}", fmt::ptr(data));
#endif

            GL_CALL(glBufferSubData(target, offset, size, data));
        }
        //-------------------------------------------------------------------------
        void get_buffer_integer_parameter(uint32 target, uint32 value, int32* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetBufferParameteriv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", buffer_target_to_string(target));
            GL_LOG("\tvalue: {0}", value);
#endif

            GL_CALL(glGetBufferParameteriv(target, value, data));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tresult data: {0}", *data);
#endif
        }

        //-------------------------------------------------------------------------
        void delete_textures(size_t count, const uint32* textures)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDeleteTextures");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\ttextures: {0}", fmt::ptr(textures));
#endif

            GL_CALL(glDeleteTextures(gsl::narrow<GLsizei>(count), textures));
        }
        //-------------------------------------------------------------------------
        void generate_textures(size_t count, uint32* textures)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGenTextures");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\ttextures: {0}", fmt::ptr(textures));
#endif

            GL_CALL(glGenTextures(gsl::narrow<GLsizei>(count), textures));
        }
        //-------------------------------------------------------------------------
        void activate_texture(uint32 texture)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glActiveTexture");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttexture: {0}", texture);
#endif

            GL_CALL(glActiveTexture(texture));
        }
        //-------------------------------------------------------------------------
        void bind_texture(uint32 target, uint32 texture)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glBindTexture");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\ttexture: {0}", texture);
#endif

            GL_CALL(glBindTexture(target, texture));
        }
        //-------------------------------------------------------------------------
        void texture_image_2D(uint32 target, int32 level, int32 internalformat, size_t width, size_t height, int32 border, uint32 format, uint32 type, const void* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexImage2D");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tlevel: {0}", level);
            GL_LOG("\tinternal format: {0}", internal_format_to_string(internalformat));
            GL_LOG("\twidth: {0}", width);
            GL_LOG("\theight: {0}", height);
            GL_LOG("\tborder: {0}", border);
            GL_LOG("\tformat: {0}", format_to_string(format));
            GL_LOG("\ttype: {0}", pixeltype_to_string(type));
            GL_LOG("\tdata: {0}", fmt::ptr(data));
#endif

            GL_CALL(glTexImage2D(target, level, internalformat, gsl::narrow<GLsizei>(width), gsl::narrow<GLsizei>(height), border, format, type, data));
        }
        //-------------------------------------------------------------------------
        void texture_image_3D(uint32 target, int32 level, int32 internalformat, size_t width, size_t height, size_t depth, int32 border, uint32 format, uint32 type, const void* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexImage3D");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tlevel: {0}", level);
            GL_LOG("\tinternal format: {0}", internal_format_to_string(internalformat));
            GL_LOG("\twidth: {0}", width);
            GL_LOG("\theight: {0}", height);
            GL_LOG("\tdepth: {0}", depth);
            GL_LOG("\tborder: {0}", border);
            GL_LOG("\tformat: {0}", format_to_string(format));
            GL_LOG("\ttype: {0}", pixeltype_to_string(type));
            GL_LOG("\tdata: {0}", fmt::ptr(data));
#endif

            GL_CALL(glTexImage3D(target, level, internalformat, gsl::narrow<GLsizei>(width), gsl::narrow<GLsizei>(height), gsl::narrow<GLsizei>(depth), border, format, type, data));
        }
        //-------------------------------------------------------------------------
        void texture_sub_image_2D(uint32 target, int32 level, int32 xoffset, int32 yoffset, size_t width, size_t height, uint32 format, uint32 type, const void* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexSubImage2D");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tlevel: {0}", level);
            GL_LOG("\txoffset: {0}", xoffset);
            GL_LOG("\tyoffset: {0}", yoffset);
            GL_LOG("\twidth: {0}", width);
            GL_LOG("\theight: {0}", height);
            GL_LOG("\tformat: {0}", format_to_string(format));
            GL_LOG("\ttype: {0}", pixeltype_to_string(type));
            GL_LOG("\tdata: {0}", fmt::ptr(data));
#endif

            GL_CALL(glTexSubImage2D(target, level, xoffset, yoffset, gsl::narrow<GLsizei>(width), gsl::narrow<GLsizei>(height), format, type, data));
        }
        //-------------------------------------------------------------------------
        void texture_sub_image_3D(uint32 target, int32 level, int32 xoffset, int32 yoffset, int32 zoffset, size_t width, size_t height, size_t depth, uint32 format, uint32 type, const void* data)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexSubImage3D");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tlevel: {0}", level);
            GL_LOG("\txoffset: {0}", xoffset);
            GL_LOG("\tyoffset: {0}", yoffset);
            GL_LOG("\tzoffet: {0}", zoffset);
            GL_LOG("\twidth: {0}", width);
            GL_LOG("\theight: {0}", height);
            GL_LOG("\tdepth: {0}", depth);
            GL_LOG("\tformat: {0}", format_to_string(format));
            GL_LOG("\ttype: {0}", pixeltype_to_string(type));
            GL_LOG("\tdata: {0}", fmt::ptr(data));
#endif

            GL_CALL(glTexSubImage3D(target, level, xoffset, yoffset, zoffset, gsl::narrow<GLsizei>(width), gsl::narrow<GLsizei>(height), gsl::narrow<GLsizei>(depth), format, type, data));
        }
        //-------------------------------------------------------------------------
        void set_texture_integer_parameter(uint32 target, uint32 pname, int32 param)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexParameteri");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tpname: {0}", pname);
            GL_LOG("\tparam: {0}", param);
#endif

            GL_CALL(glTexParameteri(target, pname, param));
        }
        //-------------------------------------------------------------------------
        void set_texture_float_parameter(uint32 target, uint32 pname, float param)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexParameterf");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tpname: {0}", pname);
            GL_LOG("\tparam: {0}", param);
#endif

            GL_CALL(glTexParameterf(target, pname, param));
        }

        //-------------------------------------------------------------------------
        void set_texture_float_array_parameter(uint32 target, uint32 pname, float* param)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glTexParameterf");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\ttarget: {0}", texture_target_to_string(target));
            GL_LOG("\tpname: {0}", pname);
            GL_LOG("\tparam: {0}", fmt::ptr(param));
#endif

            GL_CALL(glTexParameterfv(target, pname, param));
        }

        //-------------------------------------------------------------------------
        void delete_shader(uint32 shader)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDeleteShader");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshader: {0}", shader);
#endif

            GL_CALL(glDeleteShader(shader));
        }
        //-------------------------------------------------------------------------
        uint32 create_shader(uint32 shaderType)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glCreateShader");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshaderType: {0}", shadertype_to_string(shaderType));
#endif

            uint32 shader_id = 0;

            GL_CALL(shader_id = glCreateShader(shaderType));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshader id: {0}", shader_id);
#endif

            return shader_id;
        }
        //-------------------------------------------------------------------------
        void shader_source(uint32 shader, size_t count, const int8** code, const int32* length)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glShaderSource");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshader: {0}", shader);
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tcode: {0}", fmt::ptr(code));
            GL_LOG("\tlength: {0}", fmt::ptr(length));
#endif

            GL_CALL(glShaderSource(shader, gsl::narrow<GLsizei>(count), (const GLchar* const*)code, length));
        }
        //-------------------------------------------------------------------------
        void compile_shader(uint32 shader)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glCompileShader");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshader: {0}", shader);
#endif

            GL_CALL(glCompileShader(shader));
        }

        //-------------------------------------------------------------------------
        void get_shader_integer_value(uint32 shader, uint32 pname, int32* params)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetShaderiv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshader: {0}", shader);
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetShaderiv(shader, pname, params));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tparams result: {0}", *params);
#endif
        }
        //-------------------------------------------------------------------------
        void get_shader_info_log(uint32 shader, size_t maxLength, int32* length, int8* infoLog)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetShaderInfoLog");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tshader: {0}", shader);
            GL_LOG("\tmaxLength: {0}", maxLength);
            GL_LOG("\tlength: {0}", fmt::ptr(length));
            GL_LOG("\tinfoLog: {0}", fmt::ptr(infoLog));
#endif

            GL_CALL(glGetShaderInfoLog(shader, gsl::narrow<GLsizei>(maxLength), length, (GLchar*)infoLog));
        }

        //-------------------------------------------------------------------------
        void use_program(uint32 program)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUseProgram");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
#endif

            GL_CALL(glUseProgram(program));
        }
        //-------------------------------------------------------------------------
        void delete_program(uint32 program)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDeleteProgram");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
#endif

            GL_CALL(glDeleteProgram(program));
        }
        //-------------------------------------------------------------------------
        uint32 create_program()
        {
            GL_FUNC_PROFILE();

            GL_LOG("glCreateProgram");

            uint32 program_id = 0;

            GL_CALL(program_id = glCreateProgram());

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram id: {0}", program_id);
#endif

            return program_id;
        }
        //-------------------------------------------------------------------------
        void attach_shader(uint32 program, uint32 shader)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glAttachShader");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tshader: {0}", shader);
#endif

            GL_CALL(glAttachShader(program, shader));
        }
        //-------------------------------------------------------------------------
        void link_program(uint32 program)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glLinkProgram");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
#endif

            GL_CALL(glLinkProgram(program));
        }
        //-------------------------------------------------------------------------
        void detach_shader(uint32 program, uint32 shader)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glDetachShader");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tshader: {0}", shader);
#endif

            GL_CALL(glDetachShader(program, shader));
        }

        //-------------------------------------------------------------------------
        int32 get_uniform_location(uint32 program, const int8* name)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetUniformLocation");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tname: {0}", name);
#endif

            int32 location;
            GL_CALL(location = glGetUniformLocation(program, (const GLchar*)name));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
#endif

            return location;
        }
        //-------------------------------------------------------------------------
        uint32 get_uniform_block_index(uint32 program, const int8* name)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetUniformBlockIndex");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tname: {0}", name);
#endif

            uint32 index;
            GL_CALL(index = glGetUniformBlockIndex(program, (const GLchar*)name));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tindex: {0}", index);
#endif

            return index;
        }
        //-------------------------------------------------------------------------
        void get_program_integer_value(uint32 program, uint32 pname, int32* params)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetProgramiv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetProgramiv(program, pname, params));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tparams: {0}", *params);
#endif
        }
        //-------------------------------------------------------------------------
        void get_program_info_log(uint32 program, size_t maxLength, int32* length, int8* infoLog)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetProgramInfoLog");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tmax length: {0}", maxLength);
            GL_LOG("\tlength: {0}", fmt::ptr(length));
            GL_LOG("\tinfoLog: {0}", fmt::ptr(infoLog));
#endif

            GL_CALL(glGetProgramInfoLog(program, gsl::narrow<GLsizei>(maxLength), length, (GLchar*)infoLog));
        }
        //-------------------------------------------------------------------------
        void get_active_uniform(uint32 program, uint32 index, size_t bufferSize, int32* length, int32* size, uint32* type, int8* name)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetActiveUniform");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tindex: {0}", index);
            GL_LOG("\tbuffer size: {0}", bufferSize);
#endif

            GL_CALL(glGetActiveUniform(program, index, gsl::narrow<GLsizei>(bufferSize), length, size, type, (GLchar*)name));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlength: {0}", *length);
            GL_LOG("\tsize: {0}", *size);
            GL_LOG("\ttype: {0}", *type);
            GL_LOG("\tname: {0}", *name);
#endif
        }
        //-------------------------------------------------------------------------
        void get_active_uniform_block_name(uint32 program, uint32 uniformBlockIndex, size_t bufferSize, int32* length, int8* uniformBlockName)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetActiveUniformBlockName");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tuniform block index: {0}", uniformBlockIndex);
            GL_LOG("\tbuffer size: {0}", bufferSize);
#endif

            GL_CALL(glGetActiveUniformBlockName(program, uniformBlockIndex, gsl::narrow<GLsizei>(bufferSize), length, (GLchar*)uniformBlockName));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlength: {0}", *length);
            GL_LOG("\tuniform block name: {0}", uniformBlockName);
#endif
        }
        //-------------------------------------------------------------------------
        void get_active_uniform_block_integer_value(uint32 program, uint32 uniformBlockIndex, uint32 pname, int32* params)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetActiveUniformBlockiv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tuniform block index: {0}", uniformBlockIndex);
            GL_LOG("\tpname: {0}", pname);
#endif

            GL_CALL(glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tparams: {0}", *params);
#endif
        }
        //-------------------------------------------------------------------------
        void get_active_attribute(uint32 program, uint32 index, size_t bufferSize, int32* length, int32* size, uint32* type, int8* name)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glGetActiveAttrib");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tindex: {0}", index);
            GL_LOG("\tbuffer size: {0}", bufferSize);
#endif

            GL_CALL(glGetActiveAttrib(program, index, gsl::narrow<GLsizei>(bufferSize), length, size, type, (GLchar*)name));

#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlength: {0}", *length);
            GL_LOG("\tsize: {0}", *size);
            GL_LOG("\ttype: {0}", *type);
            GL_LOG("\tname: {0}", *name);
#endif
        }

        //-------------------------------------------------------------------------
        void uniform_block_binding(uint32 program, uint32 uniformBlockIndex, uint32 uniformBlockBinding)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniformBlockBinding");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tprogram: {0}", program);
            GL_LOG("\tblock index: {0}", uniformBlockIndex);
            GL_LOG("\tblock binding: {0}", uniformBlockBinding);
#endif

            GL_CALL(glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding));
        }
        //-------------------------------------------------------------------------
        void uniform_matrix_3fv(int32 location, size_t count, bool transpose, const float* value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniformMatrix3fv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\ttranspose: {0}", transpose);
            GL_LOG("\tvalue: {0}", fmt::ptr(value));
#endif

            GL_CALL(glUniformMatrix3fv(location, gsl::narrow<GLsizei>(count), transpose, value));
        }
        //-------------------------------------------------------------------------
        void uniform_matrix_4fv(int32 location, size_t count, bool transpose, const float* value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniformMatrix4fv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\ttranspose: {0}", transpose);
            GL_LOG("\tvalue: {0}", fmt::ptr(value));
#endif

            GL_CALL(glUniformMatrix4fv(location, gsl::narrow<GLsizei>(count), transpose, value));
        }
        //-------------------------------------------------------------------------
        void uniform_1f(int32 location, float value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform1f");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tvalue: {0}", value);
#endif

            GL_CALL(glUniform1f(location, value));
        }
        //-------------------------------------------------------------------------
        void uniform_2f(int32 location, float x, float y)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform2f");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
#endif

            GL_CALL(glUniform2f(location, x, y));
        }
        //-------------------------------------------------------------------------
        void uniform_3f(int32 location, float x, float y, float z)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform3f");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
            GL_LOG("\tz: {0}", z);
#endif

            GL_CALL(glUniform3f(location, x, y, z));
        }
        //-------------------------------------------------------------------------
        void uniform_4f(int32 location, float x, float y, float z, float w)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform4f");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
            GL_LOG("\tz: {0}", z);
            GL_LOG("\tw: {0}", w);
#endif

            GL_CALL(glUniform4f(location, x, y, z, w));
        }
        //-------------------------------------------------------------------------
        void uniform_1fv(int32 location, size_t count, const float* value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform1fv");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tcount: {0}", count);
            GL_LOG("\tvalue: {0}", fmt::ptr(value));
#endif

            GL_CALL(glUniform1fv(location, gsl::narrow<GLsizei>(count), value));
        }
        //-------------------------------------------------------------------------
        void uniform_1i(int32 location, int32 value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform1i");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tvalue: {0}", value);
#endif

            GL_CALL(glUniform1i(location, value));
        }
        //-------------------------------------------------------------------------
        void uniform_2i(int32 location, int32 x, int32 y)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform2i");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
#endif

            GL_CALL(glUniform2i(location, x, y));
        }
        //-------------------------------------------------------------------------
        void uniform_3i(int32 location, int32 x, int32 y, int32 z)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform3i");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
            GL_LOG("\tz: {0}", z);
#endif

            GL_CALL(glUniform3i(location, x, y, z));
        }
        //-------------------------------------------------------------------------
        void uniform_4i(int32 location, int32 x, int32 y, int32 z, int32 w)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform4i");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tx: {0}", x);
            GL_LOG("\ty: {0}", y);
            GL_LOG("\tz: {0}", z);
            GL_LOG("\tw: {0}", w);
#endif

            GL_CALL(glUniform4i(location, x, y, z, w));
        }
        //-------------------------------------------------------------------------
        void uniform_1ui(int32 location, uint32 value)
        {
            GL_FUNC_PROFILE();

            GL_LOG("glUniform1ui");
#if ENABLE_GL_PARAMETER_LOGGING && ENABLE_GL_FUNCTION_LOGGING
            GL_LOG("\tlocation: {0}", location);
            GL_LOG("\tvalue: {0}", value);
#endif

            GL_CALL(glUniform1ui(location, value));
        }
    }
}