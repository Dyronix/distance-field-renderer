#include "rex_graphics_pch.h"

#include "resources/texture_resource_manager.h"
#include "resources/texture.h"

namespace rex
{
    std::unique_ptr<TextureResourceManagerAPI> g_texture_resource_manager = nullptr;
    uint32 g_max_texture_count = 0;

    //-------------------------------------------------------------------------
    void TextureResourceManager::intialize(std::unique_ptr<TextureResourceManagerAPI> manager)
    {
        g_texture_resource_manager = std::move(manager);
    }

    //-------------------------------------------------------------------------
    uint32 TextureResourceManager::max_combined_texture_image_units()
    {
        return g_texture_resource_manager->get_max_activate_texture_count();
    }

    //-------------------------------------------------------------------------
    void TextureResourceManager::activate_texture(const ref_ptr<rex::Texture>& texture, int32 location, IsRenderThread isRenderThread)
    {
        g_texture_resource_manager->activate_texture(texture, location, isRenderThread);
    }

    //-------------------------------------------------------------------------
    void TextureResourceManager::deactivate_texture(const ref_ptr<rex::Texture>& texture, IsRenderThread isRenderThread)
    {
        g_texture_resource_manager->deactivate_texture(texture, isRenderThread);
    }

    //-------------------------------------------------------------------------
    void TextureResourceManager::release_all()
    {
        g_texture_resource_manager->release_all();
    }
}