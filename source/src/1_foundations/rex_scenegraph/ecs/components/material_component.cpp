#include "rex_scenegraph_pch.h"

#include "ecs/components/material_component.h"

#include "resources/material.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent()
            : material(nullptr)
        {
        }
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent(const MaterialComponent& other)
            : material(other.material)
        {
        }
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent(const ref_ptr<Material>& inMaterial)
            : material(inMaterial)
        {
        }

        //-------------------------------------------------------------------------
        MaterialComponent::operator ref_ptr<Material>&()
        {
            return material;
        }
        //-------------------------------------------------------------------------
        MaterialComponent::operator const ref_ptr<Material>&() const
        {
            return material;
        }

        //-------------------------------------------------------------------------
        bool MaterialComponent::operator==(const MaterialComponent& other) const
        {
            return material == other.material;
        }
        //-------------------------------------------------------------------------
        bool MaterialComponent::operator!=(const MaterialComponent& other) const
        {
            return !(*this == other);
        }
    }
}