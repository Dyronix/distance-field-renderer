#include "rex_windows_pch.h"

#include "renderer/renderer.h"

#include "resources/resource_factory.h"
#include "resources/shader_uniform_location.h"
#include "resources/shader_resource_location.h"

#include "graphics/resources/gl_shader_program.h"
#include "graphics/resources/gl_shader.h"
#include "graphics/resources/gl_uniform_buffer_set.h"

#include "string/string_operations.h"
#include "string/string_conversions.h"

#define ENABLE_SHADERCODE_LOGGING 1

#if ENABLE_SHADERCODE_LOGGING
#define SHADER_PROGRAM_TRACE R_TRACE
#define SHADER_PROGRAM_INFO R_INFO
#define SHADER_PROGRAM_WARN R_WARN
#define SHADER_PROGRAM_ERROR R_ERROR
#else
#define SHADER_PROGRAM_TRACE(...) UNUSED_PARAM(__VA_ARGS__)
#define SHADER_PROGRAM_INFO(...) UNUSED_PARAM(__VA_ARGS__)
#define SHADER_PROGRAM_WARN(...) UNUSED_PARAM(__VA_ARGS__)
#define SHADER_PROGRAM_ERROR(...) UNUSED_PARAM(__VA_ARGS__)
#endif

namespace rex
{
    //-------------------------------------------------------------------------
    std::unordered_set<StringID> g_uniforms_in_uniform_block;

    namespace opengl
    {
        using ShaderAttributeMap = std::unordered_map<StringID, ShaderAttribute>;
        using ShaderUniformMap = std::unordered_map<StringID, ShaderUniform>;
        using ShaderResourceMap = std::unordered_map<StringID, ShaderResource>;
        using ShaderUniformBlockMap = std::unordered_map<StringID, ShaderUniformBlock>;

        //-------------------------------------------------------------------------
        ShaderUniforms load_uniforms(const uint32 programID, GLint* indices, GLint uniformCount)
        {
            GLint max_count;
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_count);

            GLint size;                            // size of the variable
            GLenum type;                           // type of the variable (float, vec3 or mat4, etc)
            GLchar* name = new GLchar[max_count];  // variable name in GLSL
            GLsizei length;                        // name length

            uint32 total_size = 0;
            ShaderUniforms uniforms;
            for (int32 i = 0; i < uniformCount; ++i)
            {
                opengl::get_active_uniform(programID, (GLuint)indices[i], max_count, &length, &size, &type, (int8*)name);

                StringID uniform_sid_name = create_sid(name, length);
                ShaderUniformType uniform_type = from_open_gl_data_type(type);
                uint32 uniform_size = gsl::narrow<uint32>(conversions::to_byte_size(uniform_type).bytes());
                uint32 uniform_offset = gsl::narrow<uint32>(total_size);

                if (is_valid_type(uniform_type) == false || is_sampler_type(uniform_type) == true)
                {
                    continue;
                }

                // Store uniform names present in a uniform block.
                // This will help us track regular uniforms.
                g_uniforms_in_uniform_block.insert(uniform_sid_name);

                uniforms.push_back(ShaderUniform(uniform_sid_name, uniform_type, uniform_size, uniform_offset));

                total_size += uniform_size;
            }

            delete[] name;

            return uniforms;
        }
        //-------------------------------------------------------------------------
        ShaderUniformMap load_uniforms(const uint32 programID)
        {
            GLint count;
            GLint max_count;
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORMS, &count);
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_count);

            GLint size;                            // size of the variable
            GLenum type;                           // type of the variable (float, vec3 or mat4, etc)
            GLchar* name = new GLchar[max_count];  // variable name in GLSL
            GLsizei length;                        // name length

            uint32 total_size = 0;
            ShaderUniformMap uniforms;
            for (int32 i = 0; i < count; ++i)
            {
                opengl::get_active_uniform(programID, (GLuint)i, max_count, &length, &size, &type, (int8*)name);

                StringID uniform_sid_name = create_sid(name, length);
                ShaderUniformType uniform_type = from_open_gl_data_type(type);
                uint32 uniform_size = gsl::narrow<uint32>(conversions::to_byte_size(uniform_type).bytes());
                uint32 uniform_offset = gsl::narrow<uint32>(total_size);

                if (is_valid_type(uniform_type) == false || is_sampler_type(uniform_type) == true)
                {
                    continue;
                }

                // Check for duplicates
                if (g_uniforms_in_uniform_block.find(uniform_sid_name) != std::cend(g_uniforms_in_uniform_block))
                {
                    SHADER_PROGRAM_TRACE("\tFound duplicate uniform ({0}), use the uniform block to access this uniform", uniform_sid_name.to_string());
                    continue;
                }

                uniforms.insert(std::make_pair(uniform_sid_name, ShaderUniform(uniform_sid_name, uniform_type, uniform_size, uniform_offset)));
                total_size += uniform_size;

                // Parse array of uniforms
                if (size > 1)
                {
                    std::string uniform_name(name, length);
                    std::string::size_type pos = uniform_name.find_first_of('[');

                    R_ASSERT(pos != std::string::npos);

                    std::string base_name = uniform_sid_name.to_string().substr(0, pos);
                    for (int32 j = 1; j < size; ++j)
                    {
                        uniform_offset = total_size;

                        std::stringstream stream;
                        stream << base_name;
                        stream << "[" << j << "]";

                        std::string full_name = stream.str();
                        StringID sid_full_name = create_sid(full_name);

                        uniforms.insert(std::make_pair(sid_full_name, ShaderUniform(sid_full_name, uniform_type, uniform_size, uniform_offset)));
                        total_size += uniform_size;
                    }
                }
            }

            delete[] name;

            return uniforms;
        }
        //-------------------------------------------------------------------------
        ShaderUniformMap load_uniforms(const ShaderUniforms& uniforms)
        {
            if (uniforms.empty())
            {
                return {};
            }

            ShaderUniformMap uniforms_map;

            for (const ShaderUniform& uniform : uniforms)
            {
                if (is_valid_type(uniform.get_type()) == false || is_sampler_type(uniform.get_type()) == true)
                {
                    continue;
                }

                uniforms_map.insert(std::make_pair(uniform.get_name(), uniform));
            }

            return uniforms_map;
        }

        //-------------------------------------------------------------------------
        ShaderUniformBlock make_uniform_block(const uint32 programID, GLint blockIndex, GLint maxCount)
        {
            GLchar* name = new GLchar[maxCount];                        // variable name in GLSL
            GLsizei length = -1;                                        // name length
            GLint binding = UniformBufferSet::get_new_binding_point();  // unique binding point

            opengl::get_active_uniform_block_name(programID, (GLuint)blockIndex, maxCount, &length, (int8*)name);

            GLint data_size;
            opengl::get_active_uniform_block_integer_value(programID, (GLuint)blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &data_size);
            GLint uniform_count;
            opengl::get_active_uniform_block_integer_value(programID, (GLuint)blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniform_count);

            ShaderUniforms uniforms;
            if (uniform_count > 0)
            {
                // Load uniform indices from the active uniform block
                GLint* uniform_indices = new GLint[uniform_count];
                opengl::get_active_uniform_block_integer_value(programID, (GLuint)blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniform_indices);

                uniforms = load_uniforms(programID, uniform_indices, uniform_count);

                delete[] uniform_indices;
            }

            StringID sid_name = create_sid(name, length);

            delete[] name;

            return ShaderUniformBlock(sid_name, data_size, binding, uniforms);
        }

        //-------------------------------------------------------------------------
        ShaderUniformBlockMap load_uniform_blocks(const uint32 programID)
        {
            GLint count;
            GLint max_count;
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORM_BLOCKS, &count);
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_count);

            ShaderUniformBlockMap blocks;
            for (int32 i = 0; i < count; ++i)
            {
                ShaderUniformBlock new_block = make_uniform_block(programID, i, max_count);

                auto uniform_buffer = UniformBufferSet::instance()->get(new_block.get_name());
                if (uniform_buffer != nullptr)
                {
                    new_block = ShaderUniformBlock(uniform_buffer->get_name(), uniform_buffer->get_size(), uniform_buffer->get_binding(), uniform_buffer->get_uniforms());

                    blocks.insert(std::make_pair(new_block.get_name(), new_block));
                }
                else
                {
                    blocks.insert(std::make_pair(new_block.get_name(), new_block));
                }
            }

            return blocks;
        }
        //-------------------------------------------------------------------------
        ShaderUniformBlockMap load_uniform_blocks(const ShaderUniformBlocks& blocks)
        {
            if (blocks.empty())
            {
                return {};
            }

            ShaderUniformBlockMap blocks_map;

            for (const ShaderUniformBlock& block : blocks)
            {
                blocks_map.insert(std::make_pair(block.get_name(), block));
            }

            return blocks_map;
        }

        //-------------------------------------------------------------------------
        ShaderAttributeMap load_attributes(const uint32 programID)
        {
            GLint count;
            GLint max_count;
            opengl::get_program_integer_value(programID, GL_ACTIVE_ATTRIBUTES, &count);
            opengl::get_program_integer_value(programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_count);

            GLint size;                            // size of the variable
            GLenum type;                           // type of the variable (float, vec3 or mat4, etc)
            GLchar* name = new GLchar[max_count];  // variable name in GLSL
            GLsizei length;                        // name length

            uint32 total_size = 0;
            ShaderAttributeMap attributes;
            for (int32 i = 0; i < count; ++i)
            {
                opengl::get_active_attribute(programID, (GLuint)i, max_count, &length, &size, &type, (int8*)name);

                StringID attrib_sid_name = create_sid(name, length);
                ShaderAttributeType attrib_type = from_open_gl_data_type(type);
                uint32 attrib_size = gsl::narrow<uint32>(conversions::to_byte_size(attrib_type).bytes());
                uint32 attrib_offset = gsl::narrow<uint32>(total_size);

                attributes.insert(std::make_pair(attrib_sid_name, ShaderAttribute(attrib_sid_name, attrib_type, attrib_size, attrib_offset)));

                total_size += attrib_size;
            }

            delete[] name;

            return attributes;
        }
        //-------------------------------------------------------------------------
        ShaderAttributeMap load_attributes(const ShaderAttributes& attributes)
        {
            if (attributes.empty())
            {
                return {};
            }

            ShaderAttributeMap attributes_map;

            for (const ShaderAttribute& attrib : attributes)
            {
                attributes_map.insert(std::make_pair(attrib.get_name(), attrib));
            }

            return attributes_map;
        }

        //-------------------------------------------------------------------------
        ShaderResourceMap load_resources(const uint32 programID)
        {
            GLint count;
            GLint max_count;
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORMS, &count);
            opengl::get_program_integer_value(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_count);

            GLint size;                            // size of the variable
            GLenum type;                           // type of the variable (float, vec3 or mat4, etc)
            GLchar* name = new GLchar[max_count];  // variable name in GLSL
            GLsizei length;                        // name length

            uint32 total_size = 0;
            uint32 texture_register = 0;

            ShaderResourceMap resources;
            for (int32 i = 0; i < count; ++i)
            {
                opengl::get_active_uniform(programID, (GLuint)i, max_count, &length, &size, &type, (int8*)name);

                StringID uniform_sid_name = create_sid(name, length);
                ShaderUniformType uniform_type = from_open_gl_data_type(type);
                uint32 uniform_size = gsl::narrow<uint32>(conversions::to_byte_size(uniform_type).bytes());

                if (is_valid_type(uniform_type) == false || is_sampler_type(uniform_type) == false)
                {
                    continue;
                }

                // Check for duplicates
                if (g_uniforms_in_uniform_block.find(uniform_sid_name) != std::cend(g_uniforms_in_uniform_block))
                {
                    SHADER_PROGRAM_TRACE("\tFound duplicate uniform ({0}), use the uniform block to access this uniform", uniform_sid_name.to_string());
                    continue;
                }

                switch (uniform_type)
                {
                    case ShaderUniformType::SAMPLER2D:
                    case ShaderUniformType::SAMPLER3D:
                    case ShaderUniformType::SAMPLERCUBE:
                        resources.insert(std::make_pair(uniform_sid_name, ShaderResource(uniform_sid_name, uniform_type, 1, total_size, texture_register)));
                        ++texture_register;

                        total_size += uniform_size;
                        break;
                    default:
                        //
                        //  Even if we do not encounter a resource we still keep track of the total size.
                        //  Uniforms can be declared in any order so Samplers can be declared throughout the entire shader.
                        //
                        total_size += uniform_size;
                        continue;
                }
            }

            delete[] name;

            return resources;
        }
        //-------------------------------------------------------------------------
        ShaderResourceMap load_resources(const ShaderResources& resources)
        {
            if (resources.empty())
            {
                return {};
            }

            ShaderResourceMap resources_map;

            for (const ShaderResource& resource : resources)
            {
                if (is_valid_type(resource.get_type()) == false || is_sampler_type(resource.get_type()) == false)
                {
                    continue;
                }

                resources_map.insert(std::make_pair(resource.get_name(), resource));
            }

            return resources_map;
        }
        //-------------------------------------------------------------------------
        size_t create_hash(memory::Blob& vertexShaderCode, memory::Blob& fragmentShadercode)
        {
            std::string vertex_shader_code = std::string(vertexShaderCode.get_data_as<const char>());
            std::string fragment_shader_code = std::string(fragmentShadercode.get_data_as<const char>());

            std::string program_shader_code = vertex_shader_code + fragment_shader_code;

            return std::hash<std::string>{}(program_shader_code);
        }

        struct ShaderProgram::Internal
        {
            //-------------------------------------------------------------------------
            Internal()
                : tag(ESID::SID_None)
                , queue_index(0)
                , program_id(0)
                , is_linked(false)
                , is_bound(false)
            {
            }

            //-------------------------------------------------------------------------
            bool check_link_status()
            {
                int32 linked_status = 0;
                opengl::get_program_integer_value(program_id, GL_LINK_STATUS, (int32*)&linked_status);
                if (linked_status == GL_FALSE)
                {
                    int32 max_length = 0;
                    opengl::get_program_integer_value(program_id, GL_INFO_LOG_LENGTH, &max_length);

                    // The max_length includes the NULL character
                    std::vector<int8> info_log(max_length);
                    opengl::get_program_info_log(program_id, max_length, &max_length, &info_log[0]);

                    // We don't need the program anymore.
                    opengl::delete_program(program_id);

#pragma warning(push)
#pragma warning(disable : 4996)
                    SHADER_PROGRAM_ERROR("{0}", info_log.data());
#pragma warning(pop)
                    R_ASSERT_X(false, "Shader program link failure");

                    return false;
                }

                is_linked = true;

                return true;
            }
            //-------------------------------------------------------------------------
            bool create(const memory::Blob& vertexShaderCode, const memory::Blob& fragmentShaderCode)
            {
                SHADER_PROGRAM_TRACE("{0}", vertexShaderCode.get_data_as<char>());
                VertexShader vertex_shader(vertexShaderCode.get_data(), vertexShaderCode.get_size());
                if (!vertex_shader)
                {
                    return false;  // compilation failed
                }

                SHADER_PROGRAM_TRACE("{0}", fragmentShaderCode.get_data_as<char>());
                FragmentShader fragment_shader(fragmentShaderCode.get_data(), fragmentShaderCode.get_size());
                if (!fragment_shader)
                {
                    return false;  // compilation failed
                }

                // Vertex and fragment shaders are successfully compiled.
                // Now time to link them together into a program.
                // Get a program object.
                program_id = opengl::create_program();

                if (!program_id)
                {
                    SHADER_PROGRAM_ERROR("Could not create shader program: {0}", tag);
                    return false;
                }

                opengl::attach_shader(program_id, vertex_shader);
                opengl::attach_shader(program_id, fragment_shader);

                opengl::link_program(program_id);

                if (!check_link_status())
                {
                    return false;
                }

                opengl::detach_shader(program_id, vertex_shader);
                opengl::detach_shader(program_id, fragment_shader);

                // ORDER OF RETRIEVAL IS IMPORTANT HERE!
                // If sorting becomes more complex maybe we should add an algorithm for this.
                if (blocks.empty())
                {
                    blocks = load_uniform_blocks(program_id);
                }

                if (attributes.empty())
                {
                    attributes = load_attributes(program_id);
                }

                if (uniforms.empty())
                {
                    uniforms = load_uniforms(program_id);
                }

                if (resources.empty())
                {
                    resources = load_resources(program_id);
                }

                return true;
            }

            //-------------------------------------------------------------------------
            template <typename T>
            bool find_and_cache_uniform(const uint32& programID, const StringID& inName, std::unordered_map<StringID, T>& uniformLocations)
            {
                if (uniformLocations.find(inName) == std::end(uniformLocations))
                {
                    char* name;
                    size_t size;

                    inName.to_string(&name, size);

                    SHADER_PROGRAM_TRACE("Caching uniform: {0}, for shader program with ID: {1}", name, programID);

                    int32 location = opengl::get_uniform_location(programID, (int8*)name);
                    uniformLocations[inName] = {location};

                    if (location == -1)
                    {
                        SHADER_PROGRAM_WARN("Uniform with name: \"{0}\" was not found", name);
                        return false;
                    }
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool find_and_cache_uniformblock(const uint32& programID, const StringID& inName, std::unordered_map<StringID, int32>& uniformBlockLocations)
            {
                if (uniformBlockLocations.find(inName) == std::end(uniformBlockLocations))
                {
                    char* name;
                    size_t size;

                    inName.to_string(&name, size);

                    SHADER_PROGRAM_TRACE("Caching uniform block: {0}, for shader program with ID: {1}", name, programID);

                    int32 location = opengl::get_uniform_block_index(programID, (int8*)name);
                    uniformBlockLocations[inName] = location;

                    if (location == -1)
                    {
                        SHADER_PROGRAM_WARN("Uniform block with name: {0} was not found", name);
                        return false;
                    }
                }

                return true;
            }

            //-------------------------------------------------------------------------
            bool upload_mat3(const StringID& inName, const rex::matrix3& mat, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading MAT3 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(mat));

                if (rt)
                {
                    opengl::uniform_matrix_3fv(location, 1, false, rex::value_ptr(mat));
                }
                else
                {
                    Renderer::submit([location, mat]()
                                     {
                                         opengl::uniform_matrix_3fv(location, 1, false, rex::value_ptr(mat));
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_mat4(const StringID& inName, const rex::matrix4& mat, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading MAT4 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(mat));

                if (rt)
                {
                    opengl::uniform_matrix_4fv(location, 1, false, rex::value_ptr(mat));
                }
                else
                {
                    Renderer::submit([location, mat]()
                                     {
                                         opengl::uniform_matrix_4fv(location, 1, false, rex::value_ptr(mat));
                                     });
                }

                return true;
            }

            //-------------------------------------------------------------------------
            bool upload_float(const StringID& inName, const float value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading FLOAT uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), std::to_string(value));

                if (rt)
                {
                    opengl::uniform_1f(location, value);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_1f(location, value);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_float2(const StringID& inName, const rex::vec2& value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading FLOAT2 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(value));

                if (rt)
                {
                    opengl::uniform_2f(location, value.x, value.y);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_2f(location, value.x, value.y);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_float3(const StringID& inName, const rex::vec3& value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading FLOAT3 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(value));

                if (rt)
                {
                    opengl::uniform_3f(location, value.x, value.y, value.z);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_3f(location, value.x, value.y, value.z);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_float4(const StringID& inName, const rex::vec4& value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading FLOAT4 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(value));

                if (rt)
                {
                    opengl::uniform_4f(location, value.x, value.y, value.z, value.w);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_4f(location, value.x, value.y, value.z, value.w);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_float_array(const StringID& inName, const float* values, uint32 size, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading FLOAT ARRAY uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), fmt::ptr(values));

                if (rt)
                {
                    opengl::uniform_1fv(location, size, values);
                }
                else
                {
                    Renderer::submit([location, values, size]()
                                     {
                                         opengl::uniform_1fv(location, size, values);
                                     });
                }

                return true;
            }

            //-------------------------------------------------------------------------
            bool upload_int(const StringID& inName, const int32 value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading INT uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), std::to_string(value));

                if (rt)
                {
                    opengl::uniform_1i(location, value);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_1i(location, value);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_int2(const StringID& inName, const rex::vec2i& value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading INT2 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(value));

                if (rt)
                {
                    opengl::uniform_2i(location, value.x, value.y);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_2i(location, value.x, value.y);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_int3(const StringID& inName, const rex::vec3i& value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading INT3 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(value));

                if (rt)
                {
                    opengl::uniform_3i(location, value.x, value.y, value.z);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_3i(location, value.x, value.y, value.z);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_int4(const StringID& inName, const rex::vec4i& value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading INT4 uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), rex::to_string(value));

                if (rt)
                {
                    opengl::uniform_4i(location, value.x, value.y, value.z, value.w);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_4i(location, value.x, value.y, value.z, value.w);
                                     });
                }

                return true;
            }
            //-------------------------------------------------------------------------
            bool upload_uint(const StringID& inName, const int32 value, IsRenderThread rt)
            {
                if (!find_and_cache_uniform(program_id, inName, uniform_locations))
                {
                    return false;
                }

                int32 location = uniform_locations.at(inName);

                SHADER_PROGRAM_TRACE("Uploading UNSIGNED INT uniform. Location: {0}, Name: {1}, Value: {2}", std::to_string(location), inName.to_string(), std::to_string(value));

                if (rt)
                {
                    opengl::uniform_1ui(location, value);
                }
                else
                {
                    Renderer::submit([location, value]()
                                     {
                                         opengl::uniform_1ui(location, value);
                                     });
                }

                return true;
            }

            //-------------------------------------------------------------------------
            ShaderUniforms get_shader_uniforms() const
            {
                ShaderUniforms shader_uniforms;
                if (uniforms.empty())
                {
                    return shader_uniforms;
                }

                std::transform(std::begin(uniforms), std::end(uniforms), std::back_inserter(shader_uniforms), [](const auto& pair)
                               {
                                   return pair.second;
                               });

                return shader_uniforms;
            }
            //-------------------------------------------------------------------------
            ShaderAttributes get_shader_attributes() const
            {
                ShaderAttributes shader_attributes;
                if (attributes.empty())
                {
                    return shader_attributes;
                }

                std::transform(std::begin(attributes), std::end(attributes), std::back_inserter(shader_attributes), [](const auto& pair)
                               {
                                   return pair.second;
                               });

                return shader_attributes;
            }
            //-------------------------------------------------------------------------
            ShaderResources get_shader_resources() const
            {
                ShaderResources shader_resources;
                if (resources.empty())
                {
                    return shader_resources;
                }

                std::transform(std::begin(resources), std::end(resources), std::back_inserter(shader_resources), [](const auto& pair)
                               {
                                   return pair.second;
                               });

                return shader_resources;
            }
            //-------------------------------------------------------------------------
            ShaderUniformBlocks get_shader_uniform_blocks() const
            {
                ShaderUniformBlocks shader_blocks;
                if (blocks.empty())
                {
                    return shader_blocks;
                }

                std::transform(std::begin(blocks), std::end(blocks), std::back_inserter(shader_blocks), [](const auto& pair)
                               {
                                   return pair.second;
                               });

                return shader_blocks;
            }

            StringID tag;

            uint32 queue_index;
            uint32 program_id;
            size_t hash;

            bool is_linked;
            bool is_bound;

            std::unordered_map<StringID, ShaderUniformLocation> uniform_locations;
            std::unordered_map<StringID, ShaderResourceLocation> resource_locations;
            std::unordered_map<StringID, int32> uniformblock_locations;

            ShaderUniformMap uniforms;
            ShaderAttributeMap attributes;
            ShaderResourceMap resources;
            ShaderUniformBlockMap blocks;

            std::vector<StringID> uniform_buffers;
        };

        //-------------------------------------------------------------------------
        ShaderProgram::ShaderProgram(ShaderProgramCreationInfo&& info)
            : m_internal(rex::make_internal_scope_ptr<Internal>())
        {
            m_internal->tag = info.tag;
            m_internal->queue_index = string_conversion::string_to_int(info.queue.to_string());
            m_internal->program_id = 0;
            m_internal->is_bound = false;
            m_internal->is_linked = false;
            m_internal->attributes = load_attributes(info.attributes);
            m_internal->uniforms = load_uniforms(info.uniforms);
            m_internal->resources = load_resources(info.resources);
            m_internal->blocks = load_uniform_blocks(info.blocks);
            m_internal->hash = create_hash(info.vertexShaderCode, info.fragmentShaderCode);

            R_ASSERT(m_internal->tag != ESID::SID_None);
            R_ASSERT(m_internal->queue_index != 0);

            R_ASSERT_X(info.vertexShaderCode.get_size() != 0_bytes, "No shader code was found.");
            R_ASSERT_X(info.fragmentShaderCode.get_size() != 0_bytes, "No shader code was found");

            SHADER_PROGRAM_TRACE("[Shader Program] Compiling {0} - start", info.tag.to_string());
            if (!m_internal->create(std::move(info.vertexShaderCode), std::move(info.fragmentShaderCode)))
            {
                SHADER_PROGRAM_ERROR("Shader creation failed");
                return;
            }
            SHADER_PROGRAM_INFO("[Shader Program] Compiling {0} - success", get_name().to_string());

            for (const auto& pair : m_internal->blocks)
            {
                const ShaderUniformBlock& block = pair.second;

                if (!m_internal->find_and_cache_uniformblock(m_internal->program_id, block.get_name(), m_internal->uniformblock_locations))
                {
                    SHADER_PROGRAM_WARN("Uniform block ({0}) not found on shader ({1}).", block.get_name().to_string(), m_internal->tag.to_string());
                    continue;
                }

                opengl::uniform_block_binding(m_internal->program_id, m_internal->uniformblock_locations.at(block.get_name()), block.get_binding());

                if (UniformBufferSet::instance()->get(block.get_name()) != nullptr)
                {
                    m_internal->uniform_buffers.push_back(block.get_name());
                }
                else
                {
                    m_internal->uniform_buffers.push_back(ResourceFactory::create_uniform_buffer(block));
                }
            }

            SHADER_PROGRAM_TRACE("Uniforms found on shader \"{0}\"", m_internal->tag.to_string());
            for (const auto& pair : m_internal->uniforms)
            {
                const ShaderUniform& uniform = pair.second;

                if (!m_internal->find_and_cache_uniform(m_internal->program_id, uniform.get_name(), m_internal->uniform_locations))
                {
                    SHADER_PROGRAM_WARN("\t- Uniform ({0}) not found on shader ({1}).", uniform.get_name().to_string(), m_internal->tag.to_string());
                    continue;
                }

                SHADER_PROGRAM_TRACE("\t- Uniform {0} at location {1}", uniform.get_name().to_string(), get_uniform_location(uniform.get_name()).get());
            }

            for (const auto& pair : m_internal->resources)
            {
                const ShaderResource& resource = pair.second;

                if (!m_internal->find_and_cache_uniform(m_internal->program_id, resource.get_name(), m_internal->resource_locations))
                {
                    SHADER_PROGRAM_WARN("\t- Resource ({0}) not found on shader ({1}).", resource.get_name().to_string(), m_internal->tag.to_string());
                    continue;
                }

                SHADER_PROGRAM_TRACE("\t- Resource {0} at location {1}", resource.get_name().to_string(), get_resource_location(resource.get_name()).get());
            }
        }
        //-------------------------------------------------------------------------
        ShaderProgram::~ShaderProgram()
        {
            release();
        }

        //-------------------------------------------------------------------------
        bool ShaderProgram::is_bound() const
        {
            return m_internal->is_bound;
        }
        //-------------------------------------------------------------------------
        bool ShaderProgram::is_linked() const
        {
            return m_internal->is_linked;
        }

        //-------------------------------------------------------------------------
        const StringID& ShaderProgram::get_name() const
        {
            return m_internal->tag;
        }

        //-------------------------------------------------------------------------
        uint32 ShaderProgram::get_queue() const
        {
            return m_internal->queue_index;
        }
        //-------------------------------------------------------------------------
        uint32 ShaderProgram::get_shader_program_id() const
        {
            return m_internal->program_id;
        }

        //-------------------------------------------------------------------------
        const rex::ShaderUniform& ShaderProgram::get_uniform(const StringID& name) const
        {
            auto it = m_internal->uniforms.find(name);

            return it != std::cend(m_internal->uniforms) ? it->second : ShaderUniform::EMPTY;
        }
        //-------------------------------------------------------------------------
        const rex::ShaderUniformLocation& ShaderProgram::get_uniform_location(const StringID& name) const
        {
            auto it = m_internal->uniform_locations.find(name);

            return it != std::cend(m_internal->uniform_locations) ? it->second : ShaderUniformLocation::EMPTY;
        }
        //-------------------------------------------------------------------------
        const rex::ShaderUniforms ShaderProgram::get_uniforms() const
        {
            return m_internal->get_shader_uniforms();
        }

        //-------------------------------------------------------------------------
        const int32 ShaderProgram::get_uniform_count() const
        {
            return gsl::narrow<int32>(m_internal->uniforms.size());
        }

        //-------------------------------------------------------------------------
        const rex::ShaderAttribute& ShaderProgram::get_attribute(const StringID& name) const
        {
            auto it = m_internal->attributes.find(name);

            return it != std::cend(m_internal->attributes) ? it->second : ShaderAttribute::EMPTY;
        }
        //-------------------------------------------------------------------------
        const rex::ShaderAttributes ShaderProgram::get_attributes() const
        {
            return m_internal->get_shader_attributes();
        }

        //-------------------------------------------------------------------------
        const int32 ShaderProgram::get_attribute_count() const
        {
            return gsl::narrow<int32>(m_internal->attributes.size());
        }

        //-------------------------------------------------------------------------
        const rex::ShaderResource& ShaderProgram::get_resource(const StringID& name) const
        {
            auto it = m_internal->resources.find(name);

            return it != std::cend(m_internal->resources) ? it->second : ShaderResource::EMPTY;
        }
        //-------------------------------------------------------------------------
        const rex::ShaderResourceLocation& ShaderProgram::get_resource_location(const StringID& name) const
        {
            auto it = m_internal->resource_locations.find(name);

            if (it != std::cend(m_internal->resource_locations))
            {
                return it->second;
            }

            return ShaderResourceLocation::EMPTY;
        }
        //-------------------------------------------------------------------------
        const rex::ShaderResources ShaderProgram::get_resources() const
        {
            return m_internal->get_shader_resources();
        }

        //-------------------------------------------------------------------------
        const int32 ShaderProgram::get_resource_count() const
        {
            return gsl::narrow<int32>(m_internal->resources.size());
        }

        //-------------------------------------------------------------------------
        const rex::ShaderUniformBlock& ShaderProgram::get_uniform_block(const StringID& name) const
        {
            auto it = m_internal->blocks.find(name);

            return it != std::cend(m_internal->blocks) ? it->second : ShaderUniformBlock::EMPTY;
        }
        //-------------------------------------------------------------------------
        const rex::ShaderUniformBlocks ShaderProgram::get_uniform_blocks() const
        {
            return m_internal->get_shader_uniform_blocks();
        }

        //-------------------------------------------------------------------------
        const int32 ShaderProgram::get_uniform_block_count() const
        {
            return gsl::narrow<int32>(m_internal->blocks.size());
        }

        //-------------------------------------------------------------------------
        void ShaderProgram::release()
        {
            if (m_internal->program_id)
            {
                opengl::delete_program(m_internal->program_id);
                m_internal->program_id = 0;
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::bind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                R_ASSERT(m_internal->program_id != 0);

                opengl::use_program(m_internal->program_id);

                m_internal->is_bound = true;
            }
            else
            {
                ref_ptr<const ShaderProgram> instance(this);
                Renderer::submit([instance]() mutable
                                 {
                                     R_ASSERT(instance->m_internal->program_id != 0);

                                     opengl::use_program(instance->m_internal->program_id);
                                     instance->m_internal->is_bound = true;
                                 });
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::unbind(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            if (rt)
            {
                opengl::use_program(0);
                m_internal->is_bound = false;
            }
            else
            {
                ref_ptr<const ShaderProgram> instance(this);
                Renderer::submit([instance]()
                                 {
                                     opengl::use_program(0);
                                     instance->m_internal->is_bound = false;
                                 });
            }
        }

        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::matrix3& mat, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"mat3\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_mat3(name, mat, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::matrix4& mat, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"mat4\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_mat4(name, mat, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }

        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const float value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"float\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_float(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::vec2& value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"vec2\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_float2(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::vec3& value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"vec3\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_float3(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::vec4& value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"vec4\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_float4(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }

        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::vec2i& value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"ivec2\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_int2(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::vec3i& value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"ivec3\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_int3(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const rex::vec4i& value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"ivec4\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_int4(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }

        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const float* values, uint32 size, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"uint32\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_float_array(name, values, size, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }

        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const int32 value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"int32\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_int(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const uint32 value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"uint32\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_uint(name, value, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
        //-------------------------------------------------------------------------
        void ShaderProgram::set_uniform(const StringID& name, const bool value, IsRenderThread rt /* = IsRenderThread::NO */) const
        {
            if (!is_bound() && rt)
            {
                R_WARN("Uploading uniform \"bool\" when shader is not bound");
                return;
            }

            if (!m_internal->upload_int(name, value ? 1 : 0, rt))
            {
                R_WARN("Uniform ({0}) not found on shader ({1})", name.to_string(), m_internal->tag.to_string());
            }
        }
    }
}