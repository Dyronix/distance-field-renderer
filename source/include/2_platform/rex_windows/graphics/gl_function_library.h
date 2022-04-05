#pragma once

#include "point.h"

namespace rex
{
    namespace opengl
    {
        void enable_vertex_attrib_array(uint32 index);
        void vertex_attrib_i_pointer(uint32 index, int32 size, uint32 type, size_t stride, const void* ptr);
        void vertex_attrib_pointer(uint32 index, int32 size, uint32 type, bool normalized, size_t stride, const void* ptr);
        void delete_vertex_arrays(size_t count, const uint32* arrays);
        void generate_vertex_arrays(size_t count, uint32* arrays);
        void bind_vertex_array(uint32 arrayID);

        void viewport(int32 x, int32 y, int32 width, int32 height);

        void clear_color(float red, float green, float blue, float alpha);
        void clear_depth(double depth);
        void clear_depth_f(float depth);
        void clear(uint32 bitfield);

        void enable(uint32 value);
        void disable(uint32 value);

        void cull_face(uint32 mode);
        void front_face(uint32 mode);
        void depth_func(uint32 fn);
        void depth_mask(uint8 mode);
        void polygon_mode(uint32 face, uint32 mode);

        void draw_elements(uint32 mode, size_t count, uint32 type, const void* indices);
        void draw_elements_base_vertex(uint32 mode, size_t count, uint32 type, const void* indices, int32 baseVertex);
        void draw_arrays(uint32 mode, int32 first, size_t count);

        void get_integer_value(uint32 pname, int32* data);
        void get_float_value(uint32 pname, float* data);
        void get_double_value(uint32 pname, double* data);
        void get_bool_value(uint32 pname, uint8* data);
        const char* get_string_value(uint32 name);
        uint32 get_error();

        uint32 check_framebuffer_status(uint32 target);
        void delete_framebuffers(size_t count, const uint32* framebuffers);
        void generate_framebuffers(size_t count, uint32* framebuffers);
        void bind_framebuffer(uint32 target, int32 index);
        void blit_framebuffer(int32 srcx, int32 srcy, int32 srcWidth, int32 srcHeight, int32 dstx, int32 dsty, int32 dstWidth, int32 dstHeight, uint32 mask, uint32 filter);
        void framebuffer_texture2D(uint32 target, uint32 attachment, uint32 textureTarget, uint32 texture, int32 level);
        void draw_buffer(uint32 buffer);
        void draw_buffers(size_t count, const uint32* buffers);
        void get_framebuffer_integer_parameter(uint32 target, uint32 value, int32* data);

        void delete_buffers(size_t count, const uint32* buffers);
        void generate_buffers(size_t count, uint32* buffers);
        void bind_buffer(uint32 target, uint32 index);
        void buffer_data(uint32 target, uint32 size, const void* data, uint32 usage);
        void bind_buffer_range(uint32 target, uint32 index, uint32 buffer, int64 offset, int64 size);
        void buffer_sub_data(uint32 target, int64 offset, int64 size, const void* data);
        void get_buffer_integer_parameter(uint32 target, uint32 value, int32* data);

        void delete_textures(size_t count, const uint32* textures);
        void generate_textures(size_t count, uint32* textures);
        void activate_texture(uint32 texture);
        void bind_texture(uint32 target, uint32 texture);
        void texture_image_2D(uint32 target, int32 level, int32 internalformat, size_t width, size_t height, int32 border, uint32 format, uint32 type, const void* data);
        void texture_image_3D(uint32 target, int32 level, int32 internalformat, size_t width, size_t height, size_t depth, int32 border, uint32 format, uint32 type, const void* data);
        void texture_sub_image_2D(uint32 target, int32 level, int32 xoffset, int32 yoffset, size_t width, size_t height, uint32 format, uint32 type, const void* data);
        void texture_sub_image_3D(uint32 target, int32 level, int32 xoffset, int32 yoffset, int32 zoffset, size_t width, size_t height, size_t depth, uint32 format, uint32 type, const void* data);
        void set_texture_integer_parameter(uint32 target, uint32 pname, int32 param);
        void set_texture_float_parameter(uint32 target, uint32 pname, float param);
        void set_texture_float_array_parameter(uint32 target, uint32 pname, float* param);

        void delete_shader(uint32 shader);
        uint32 create_shader(uint32 shaderType);
        void shader_source(uint32 shader, size_t count, const int8** code, const int32* length);
        void compile_shader(uint32 shader);

        void get_shader_integer_value(uint32 shader, uint32 pname, int32* params);
        void get_shader_info_log(uint32 shader, size_t maxLength, int32* length, int8* infoLog);

        void use_program(uint32 program);
        void delete_program(uint32 program);
        uint32 create_program();
        void attach_shader(uint32 program, uint32 shader);
        void link_program(uint32 program);
        void detach_shader(uint32 program, uint32 shader);

        int32 get_uniform_location(uint32 program, const int8* name);
        uint32 get_uniform_block_index(uint32 program, const int8* name);
        void get_program_integer_value(uint32 shader, uint32 pname, int32* params);
        void get_program_info_log(uint32 shader, size_t maxLength, int32* length, int8* infoLog);
        void get_active_uniform(uint32 program, uint32 index, size_t bufferSize, int32* length, int32* size, uint32* type, int8* name);
        void get_active_uniform_block_name(uint32 program, uint32 uniformBlockIndex, size_t bufferSize, int32* length, int8* uniformBlockName);
        void get_active_uniform_block_integer_value(uint32 program, uint32 uniformBlockIndex, uint32 pname, int32* params);
        void get_active_attribute(uint32 program, uint32 index, size_t bufferSize, int32* length, int32* size, uint32* type, int8* name);

        void uniform_block_binding(uint32 program, uint32 uniformBlockIndex, uint32 uniformBlockBinding);
        void uniform_matrix_3fv(int32 location, size_t count, bool transpose, const float* value);
        void uniform_matrix_4fv(int32 location, size_t count, bool transpose, const float* value);
        void uniform_1f(int32 location, float value);
        void uniform_2f(int32 location, float x, float y);
        void uniform_3f(int32 location, float x, float y, float z);
        void uniform_4f(int32 location, float x, float y, float z, float w);
        void uniform_1fv(int32 location, size_t count, const float* value);
        void uniform_1i(int32 location, int32 x);
        void uniform_2i(int32 location, int32 x, int32 y);
        void uniform_3i(int32 location, int32 x, int32 y, int32 z);
        void uniform_4i(int32 location, int32 x, int32 y, int32 z, int32 w);
        void uniform_1ui(int32 location, uint32 x);
    }
}