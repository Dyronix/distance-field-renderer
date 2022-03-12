#include "rex_windows_pch.h"

#include "graphics/resources/gl_texture_resource_manager.h"

#include "resources/texture.h"

#include "renderer/renderer.h"

namespace rex
{
    namespace opengl
    {
        //-------------------------------------------------------------------------
        struct ActiveTexture
        {
            uint32 texture_slot;
            uint32 texture_id;

            ref_ptr<Texture> texture;
        };

        //-------------------------------------------------------------------------
        using ActiveTextureVector = std::vector<ActiveTexture>;

        uint32                  g_max_texture_count     = 0;
        ActiveTextureVector     g_activated_textures    = {};

        //-------------------------------------------------------------------------
        TextureResourceManagerAPI::TextureResourceManagerAPI(uint32 maxTextureCount)
        {
            g_max_texture_count = maxTextureCount;
        }
        //-------------------------------------------------------------------------
        TextureResourceManagerAPI::~TextureResourceManagerAPI()
        {
            release_all();
        }

        //-------------------------------------------------------------------------
        uint32 TextureResourceManagerAPI::get_activated_texture_count() const
        {
            return static_cast<uint32>(g_activated_textures.size());
        }
        //-------------------------------------------------------------------------
        uint32 TextureResourceManagerAPI::get_max_activate_texture_count() const
        {
            return g_max_texture_count;
        }

        //-------------------------------------------------------------------------
        void TextureResourceManagerAPI::activate_texture(const ref_ptr<rex::Texture>& texture, int32 location, IsRenderThread rt)
        {
            if (texture)
            {
                R_ASSERT_X(g_activated_textures.size() <= g_max_texture_count, "Maximum texture slots reached for this draw call, flush before progressing");

                uint32 offset = GL_TEXTURE1 - GL_TEXTURE0;
                uint32 available_slot = GL_TEXTURE0 + (location * offset);

                if (rt)
                {
                    opengl::activate_texture(available_slot);
                    texture->bind(rt);
                }
                else
                {
                    Renderer::submit([available_slot, texture]()
                        {
                            opengl::activate_texture(available_slot);
                            texture->bind(IsRenderThread::YES);
                        });
                }

                ActiveTexture activate_texture;

                activate_texture.texture_slot = available_slot;
                activate_texture.texture_id = texture->get_id();
                activate_texture.texture = texture;

                g_activated_textures.push_back(activate_texture);
            }
        }
        //-------------------------------------------------------------------------
        void TextureResourceManagerAPI::deactivate_texture(const ref_ptr<rex::Texture>& texture, IsRenderThread rt)
        {
            auto it = std::find_if(std::cbegin(g_activated_textures), std::cend(g_activated_textures),
                [id = texture->get_id()](const ActiveTexture& activeTexture)
                {
                    return id == activeTexture.texture_id;
                });

            if (it == std::cend(g_activated_textures))
            {
                R_WARN("Texture with name: {0} is not managed by the texture manager", texture->get_name().to_string());
                return;
            }

            if (rt)
            {
                opengl::activate_texture(it->texture_slot);
                it->texture->unbind();
            }
            else
            {
                Renderer::submit([available_slot = it->texture_slot, texture = it->texture]()
                    {
                        opengl::activate_texture(available_slot);
                        texture->unbind(IsRenderThread::YES);
                    });
            }

            g_activated_textures.erase(it);
        }

        //-------------------------------------------------------------------------
        void TextureResourceManagerAPI::release_all()
        {
            for (auto active_texture : g_activated_textures)
            {
                opengl::activate_texture(active_texture.texture_slot);
                active_texture.texture->unbind(IsRenderThread::YES);
            }

            g_activated_textures.clear();
        }

    }
}