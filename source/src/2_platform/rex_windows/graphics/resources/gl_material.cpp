#include "rex_windows_pch.h"

#include "graphics/resources/gl_material.h"

#include "renderer/renderer.h"

#include "resources/texture.h"
#include "resources/texture_cube.h"
#include "resources/texture_resource_manager.h"
#include "resources/shader_uniform.h"
#include "resources/shader_resource.h"
#include "resources/shader_program.h"

#include "data_type_helper.h"

#include "blobreader.h"
#include "blobwriter.h"

namespace rex
{
    namespace opengl
    {
        using TextureSampler = uint32;

        struct Material::Internal
        {
            //-------------------------------------------------------------------------
            Internal(ref_ptr<rex::ShaderProgram> shader, const StringID& name)
                : material_shader(shader)
                , material_name(name)
                , flags((int32)MaterialFlag::None)
            {
                if (shader == nullptr)
                {
                    R_ERROR("Creating material with empty shader: {0}", name.to_string());
                }

                allocateUniformBufferStorage();

                flags |= (uint32_t)MaterialFlag::DepthTest;
                flags |= (uint32_t)MaterialFlag::Blend;
            }
            ~Internal()
            {
                material_shader->release();
                material_shader.reset();
            }

            //-------------------------------------------------------------------------
            void flush_uniforms(IsRenderThread rt)
            {
                auto& buffer = uniform_storage_buffer;
                if (buffer.get_size() == 0_bytes)
                {
                    return;
                }

                auto uniforms = material_shader->get_uniforms();
                for (auto& uniform : uniforms)
                {
                    auto& name = uniform.get_name();

                    switch (uniform.get_type())
                    {
                        case ShaderUniformType::BOOL:
                        case ShaderUniformType::UNSIGNED_INT32:
                        {
                            const uint32_t value = memory::reader::read<uint32>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::INT32:
                        {
                            const int value = memory::reader::read<int32>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::IVEC2:
                        {
                            const rex::vec2i& value = memory::reader::read<rex::vec2i>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::IVEC3:
                        {
                            const rex::vec3i& value = memory::reader::read<rex::vec3i>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::IVEC4:
                        {
                            const rex::vec4i& value = memory::reader::read<rex::vec4i>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::FLOAT32:
                        {
                            const float value = memory::reader::read<float>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::VEC2:
                        {
                            const rex::vec2& value = memory::reader::read<rex::vec2>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::VEC3:
                        {
                            const rex::vec3& value = memory::reader::read<rex::vec3>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::VEC4:
                        {
                            const rex::vec4& value = memory::reader::read<rex::vec4>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::MAT3:
                        {
                            const rex::matrix3& value = memory::reader::read<rex::matrix3>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        case ShaderUniformType::MAT4:
                        {
                            const rex::matrix4& value = memory::reader::read<rex::matrix4>(buffer, uniform.get_offset());
                            material_shader->set_uniform(name, value, rt);
                            break;
                        }
                        default:
                        {
                            R_WARN("Shader Uniform Type: \"{0}\" is not a uniform type", conversions::to_display_string(uniform.get_type()));
                            break;
                        }
                    }
                }
            }
            //-------------------------------------------------------------------------
            void flush_resources(IsRenderThread rt)
            {
                auto resources = material_shader->get_resources();
                if (resources.size() == 0)
                {
                    return;
                }

                R_ASSERT(textures.size() > 0 || texture_cubes.size() > 0);

                int32 texture_units = 0;
                texture_units = TextureResourceManager::max_combined_texture_image_units();

                for (auto& resource : resources)
                {
                    const auto resource_name = resource.get_name();
                    const auto resource_register = (int32)resource.get_register();

                    switch (resource.get_type())
                    {
                        case ShaderUniformType::SAMPLER2D:
                        {
                            if (resource_register > texture_units)
                            {
                                R_WARN("Resource with name: {0}, not set. Sampler Location ( {1} ) > Max Texture Units ( {2} )", resource_name.to_string(), resource_register, texture_units);
                                break;
                            }

                            R_TRACE("[MATERIAL: {0}] Binding texture 2D with name {1} to slot {2}", material_name.to_string(), resource_name.to_string(), resource_register);

                            material_shader->set_uniform(resource_name, resource_register, rt);
                            TextureResourceManager::activate_texture(ref_ptr<Texture>(textures[resource_register]), resource_register, rt);
                            break;
                        }
                        case ShaderUniformType::SAMPLER3D:
                        {
                            if (resource_register > texture_units)
                            {
                                R_WARN("Resource with name: {0}, not set. Sampler Location ( {1} ) > Max Texture Units ( { 2 } )", resource_name.to_string(), resource_register, texture_units);
                                break;
                            }

                            R_TRACE("[MATERIAL: {0}] Binding texture 3D with name {1} to slot {2}", material_name.to_string(), resource_name.to_string(), resource_register);

                            material_shader->set_uniform(resource_name, resource_register, rt);
                            TextureResourceManager::activate_texture(ref_ptr<Texture>(textures[resource_register]), resource_register, rt);
                            break;
                        }
                        case ShaderUniformType::SAMPLERCUBE:
                        {
                            if (resource_register > texture_units)
                            {
                                R_WARN("Resource with name: {0}, not set. Sampler Location ( {1} ) > Max Texture Units ( { 2 } )", resource_name.to_string(), resource_register, texture_units);
                                break;
                            }

                            R_TRACE("[MATERIAL: {0}] Binding texture CUBE with name {1} to slot {2}", material_name.to_string(), resource_name.to_string(), resource_register);

                            material_shader->set_uniform(resource_name, resource_register, rt);
                            TextureResourceManager::activate_texture(ref_ptr<Texture>(texture_cubes[resource_register]), resource_register, rt);
                            break;
                        }

                        default:
                        {
                            R_WARN("Shader Uniform Type: \"{0}\" is not a resource type", conversions::to_display_string(resource.get_type()));
                            break;
                        }
                    }
                }
            }

            //-------------------------------------------------------------------------
            void allocateUniformBufferStorage()
            {
                const auto& uniforms = material_shader->get_uniforms();
                const auto& resources = material_shader->get_resources();

                uint32 total_size = 0;
                if (uniforms.size() > 0)
                {
                    for (const ShaderUniform& uniform : uniforms)
                    {
                        total_size += uniform.get_size();
                    }
                }

                if (resources.size() > 0)
                {
                    for (int32 i = 0; i < gsl::narrow<int32>(resources.size()); ++i)
                    {
                        total_size += sizeof(TextureSampler);
                    }
                }

                uniform_storage_buffer.allocate(total_size);
                uniform_storage_buffer.zero_initialize();
            }

            //-------------------------------------------------------------------------
            const ShaderUniform& find_uniform_declaration(const StringID& name)
            {
                return material_shader->get_uniform(name);
            }
            //-------------------------------------------------------------------------
            const ShaderResource& find_resource_declaration(const StringID& name)
            {
                return material_shader->get_resource(name);
            }

            //-------------------------------------------------------------------------
            template <typename T>
            void set(const StringID& name, const T& value)
            {
                const auto& decl = find_uniform_declaration(name);
                if (!decl.is_valid())
                {
                    R_WARN("[MATERIAL: {0}] Could not find uniform with name: {1}", material_name.to_string(), name.to_string());
                    return;
                }

#if defined REX_DEBUG
                if (datatype_helper::is_of_type<T>(decl.get_type()) == false)
                {
                    auto given_type = datatype_helper::get_type<T>();
                    auto expected = decl.get_type();

                    R_ERROR("Incorrect type was querried, {0} was given but {1} is expected.", conversions::to_display_string(given_type), conversions::to_display_string(expected));
                    return;
                }
#endif

                memory::writer::write(uniform_storage_buffer, (memory::byte*)&value, decl.get_size(), decl.get_offset());
            }
            //-------------------------------------------------------------------------
            void set_texture(const StringID& name, const ref_ptr<Texture>& texture)
            {
                const auto& decl = find_resource_declaration(name);
                if (!decl.is_valid())
                {
                    R_WARN("[MATERIAL: {0}] Cannot find resource with name: {1}", material_name.to_string(), name.to_string());
                    return;
                }

                const int32 texture_register = (int32)decl.get_register();

                R_TRACE("[MATERIAL: {0}] Caching texture register of texture \"{1}\" with value: {2}", material_name.to_string(), name.to_string(), texture_register);

                textures[texture_register] = texture;
            }
            //-------------------------------------------------------------------------
            void set_texture_cube(const StringID& name, const ref_ptr<TextureCube>& texture)
            {
                const auto& decl = find_resource_declaration(name);
                if (!decl.is_valid())
                {
                    R_WARN("[MATERIAL: {0}] Cannot find resource with name: {1}", material_name.to_string(), name.to_string());
                    return;
                }

                const int32 texture_register = (int32)decl.get_register();

                R_TRACE("[MATERIAL: {0}] Caching texture register of texture \"{1}\" with value: {2}", material_name.to_string(), name.to_string(), texture_register);

                texture_cubes[texture_register] = texture;
            }

            //-------------------------------------------------------------------------
            template <typename T>
            T get(const StringID& name)
            {
                const auto& decl = find_resource_declaration(name);
                R_ASSERT_X(decl.is_valid(), "Could not find uniform");

                return memory::reader::read<T>(uniform_storage_buffer, decl.get_offset());
            }
            //-------------------------------------------------------------------------
            const ref_ptr<Texture> get_texture(const StringID& name)
            {
                const auto& decl = find_resource_declaration(name);
                if (decl.is_valid())
                {
                    R_WARN("[MATERIAL: {0}] Cannot find resource with name: {1}", material_name.to_string(), name.to_string());
                    return nullptr;
                }

                uint32 slot = decl.get_register();

                R_ASSERT_X(slot < textures.size(), "Texture slot is invalid!");

                return textures[slot];
            }
            //-------------------------------------------------------------------------
            const ref_ptr<TextureCube> get_texture_cube(const StringID& name)
            {
                const auto& decl = find_resource_declaration(name);
                if (decl.is_valid())
                {
                    R_WARN("[MATERIAL: {0}] Cannot find resource with name: {1}", material_name.to_string(), name.to_string());
                    return nullptr;
                }

                uint32 slot = decl.get_register();

                R_ASSERT_X(slot < texture_cubes.size(), "Texture slot is invalid!");

                return texture_cubes[slot];
            }

            StringID material_name;

            ref_ptr<rex::ShaderProgram> material_shader;

            memory::Blob uniform_storage_buffer;

            std::map<uint32_t, ref_ptr<Texture>> textures;
            std::map<uint32_t, ref_ptr<TextureCube>> texture_cubes;

            uint32_t flags;
        };

        //-------------------------------------------------------------------------
        Material::Material(ref_ptr<rex::ShaderProgram> shader, const StringID& name)
            : m_internal(rex::make_internal_scope_ptr<Internal>(shader, name))
        {
        }
        //-------------------------------------------------------------------------
        Material::~Material()
        {
        }

        //-------------------------------------------------------------------------
        void Material::flush(IsRenderThread rt /*= IsRenderThread::NO*/) const
        {
            m_internal->material_shader->bind(rt);

            m_internal->flush_uniforms(rt);
            m_internal->flush_resources(rt);
        }

        //-------------------------------------------------------------------------
        void Material::set_flag(MaterialFlag flag)
        {
            m_internal->flags |= (uint32)flag;
        }
        //-------------------------------------------------------------------------
        void Material::clear_flag(MaterialFlag flag)
        {
            m_internal->flags &= ~(uint32)flag;
        }
        //-------------------------------------------------------------------------
        uint32 Material::get_flags() const
        {
            return m_internal->flags;
        }
        //-------------------------------------------------------------------------
        bool Material::has_flag(MaterialFlag flag) const
        {
            return (uint32)flag & m_internal->flags;
        }

        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, float value)
        {
            m_internal->set<float>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, int32 value)
        {
            m_internal->set<int32>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, uint32 value)
        {
            m_internal->set<uint32>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, bool value)
        {
            m_internal->set<int32>(name, value ? 1 : 0);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::vec2& value)
        {
            m_internal->set<rex::vec2>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::vec3& value)
        {
            m_internal->set<rex::vec3>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::vec4& value)
        {
            m_internal->set<rex::vec4>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::vec2i& value)
        {
            m_internal->set<rex::vec2i>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::vec3i& value)
        {
            m_internal->set<rex::vec3i>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::vec4i& value)
        {
            m_internal->set<rex::vec4i>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::matrix3& value)
        {
            m_internal->set<rex::matrix3>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set(const StringID& name, const rex::matrix4& value)
        {
            m_internal->set<rex::matrix4>(name, value);
        }
        //-------------------------------------------------------------------------
        void Material::set_texture2d(const StringID& name, const ref_ptr<Texture>& texture)
        {
            m_internal->set_texture(name, texture);
        }

        //-------------------------------------------------------------------------
        void Material::set_texture3d(const StringID& name, const ref_ptr<Texture>& texture)
        {
            m_internal->set_texture(name, texture);
        }

        //-------------------------------------------------------------------------
        void Material::set_texture_cube(const StringID& name, const ref_ptr<TextureCube>& texture)
        {
            m_internal->set_texture_cube(name, texture);
        }

        //-------------------------------------------------------------------------
        float Material::get_float(const StringID& name)
        {
            return m_internal->get<float>(name);
        }
        //-------------------------------------------------------------------------
        int32 Material::get_int(const StringID& name)
        {
            return m_internal->get<int32>(name);
        }
        //-------------------------------------------------------------------------
        uint32 Material::get_u_int(const StringID& name)
        {
            return m_internal->get<uint32>(name);
        }
        //-------------------------------------------------------------------------
        bool Material::get_bool(const StringID& name)
        {
            return m_internal->get<bool>(name);
        }
        //-------------------------------------------------------------------------
        rex::vec2 Material::get_vector2(const StringID& name)
        {
            return m_internal->get<rex::vec2>(name);
        }
        //-------------------------------------------------------------------------
        rex::vec3 Material::get_vector3(const StringID& name)
        {
            return m_internal->get<rex::vec3>(name);
        }
        //-------------------------------------------------------------------------
        rex::vec4 Material::get_vector4(const StringID& name)
        {
            return m_internal->get<rex::vec4>(name);
        }
        //-------------------------------------------------------------------------
        rex::matrix3 Material::get_matrix3(const StringID& name)
        {
            return m_internal->get<rex::matrix3>(name);
        }
        //-------------------------------------------------------------------------
        rex::matrix4 Material::get_matrix4(const StringID& name)
        {
            return m_internal->get<rex::matrix4>(name);
        }
        //-------------------------------------------------------------------------
        const ref_ptr<rex::Texture> Material::get_texture2d(const StringID& name)
        {
            return m_internal->get_texture(name);
        }

        //-------------------------------------------------------------------------
        const rex::ref_ptr<rex::Texture> Material::get_texture3d(const StringID& name)
        {
            return m_internal->get_texture(name);
        }

        //-------------------------------------------------------------------------
        const rex::ref_ptr<rex::TextureCube> Material::get_texture_cube(const StringID& name)
        {
            return m_internal->get_texture_cube(name);
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::ShaderProgram>& Material::get_shader_program()
        {
            return m_internal->material_shader;
        }

        //-------------------------------------------------------------------------
        const ref_ptr<rex::ShaderProgram>& Material::get_shader_program() const
        {
            return m_internal->material_shader;
        }

        //-------------------------------------------------------------------------
        const rex::StringID& Material::get_name() const
        {
            return m_internal->material_name;
        }

        //-------------------------------------------------------------------------
        const rex::memory::Blob& Material::get_uniform_buffer_storage() const
        {
            return m_internal->uniform_storage_buffer;
        }
    }
}