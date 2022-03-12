#include "rex_graphics_pch.h"
#include "resources/texture_cube.h"
#include "resources/resource_factory.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Texture::Type TextureCube::get_type() const
    {
        return Texture::Type::TEXTURE_CUBE;
    }

    //-------------------------------------------------------------------------
    rex::SolidColorTextureCube::SolidColorTextureCube(rgba* textureData, uint32 size)
    {
        const uint32 data_width = 1u;
        const uint32 data_height = 1u;

        texture = ResourceFactory::create_texture_cube(textureData, sizeof(rgba) * size, data_width, data_height, "Solid Color Texture Cube");
    }

    //-------------------------------------------------------------------------
    void rex::SolidColorTextureCube::release()
    {
        texture->release();
        texture.reset();
    }

    //-------------------------------------------------------------------------
    ref_ptr<TextureCube>& rex::SolidColorTextureCube::get_texture()
    {
        return texture;
    }
    //-------------------------------------------------------------------------
    const ref_ptr<TextureCube>& rex::SolidColorTextureCube::get_texture() const
    {
        return texture;
    }
}