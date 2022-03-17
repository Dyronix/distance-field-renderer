#include "rex_scenegraph_pch.h"

#include "ecs/components/material_component.h"

#include "resources/material.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent()
            : materials()
        {
        }
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent(const MaterialComponent& other)
            : materials(other.materials)
        {
        }
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent(MaterialComponent&& other) noexcept
            : materials(std::exchange(other.materials, {}))
        {
        }
        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent(const ref_ptr<Material>& inMaterial)
            : materials()
        {
            add(inMaterial);
        }

        //-------------------------------------------------------------------------
        MaterialComponent::MaterialComponent(const std::vector<ref_ptr<Material>>& inMaterials)
            : materials()
        {
            for (const auto& m : inMaterials)
            {
                add(m);
            }
        }

        //-------------------------------------------------------------------------
        rex::ecs::MaterialComponent& MaterialComponent::operator=(const MaterialComponent& other)
        {
            this->materials = other.materials;
            return *this;
        }

        //-------------------------------------------------------------------------
        rex::ecs::MaterialComponent& MaterialComponent::operator=(MaterialComponent&& other) noexcept
        {
            this->materials = std::exchange(other.materials, {});
            return *this;
        }

        //-------------------------------------------------------------------------
        MaterialComponent::operator std::vector<std::tuple<int32, ref_ptr<Material>>>&()
        {
            return materials;
        }
        //-------------------------------------------------------------------------
        MaterialComponent::operator const std::vector<std::tuple<int32, ref_ptr<Material>>>&() const
        {
            return materials;
        }

        //-------------------------------------------------------------------------
        bool MaterialComponent::operator==(const MaterialComponent& other) const
        {
            return materials == other.materials;
        }
        //-------------------------------------------------------------------------
        bool MaterialComponent::operator!=(const MaterialComponent& other) const
        {
            return !(*this == other);
        }

        //-------------------------------------------------------------------------
        bool MaterialComponent::add(const ref_ptr<Material>& inMaterial)
        {
            auto it = std::find_if(std::cbegin(materials), std::cend(materials),
                                   [name = inMaterial->get_name()](const auto& tuple)
                                   {
                                       int32 id;
                                       ref_ptr<Material> material;

                                       std::tie(id, material) = tuple;

                                       return name == material->get_name();
                                   });

            if (it != std::cend(materials))
            {
                R_WARN("Material ({0}) already present", inMaterial->get_name());
                return false;
            }

            materials.push_back(std::make_tuple(gsl::narrow<int32>(materials.size()), inMaterial));
            return true;
        }

        //-------------------------------------------------------------------------
        bool MaterialComponent::set(int32 index, const ref_ptr<Material>& inMaterial)
        {
            if (materials.size() < index)
            {
                return add(inMaterial);
            }

            auto it = std::find_if(std::cbegin(materials), std::cend(materials),
                                   [name = inMaterial->get_name()](const auto& tuple)
                                   {
                                       int32 id;
                                       ref_ptr<Material> material;

                                       std::tie(id, material) = tuple;

                                       return name == material->get_name();
                                   });

            if (it != std::cend(materials))
            {
                R_WARN("Material ({0}) already present", inMaterial->get_name());
                return false;
            }
            
            materials[index] = std::make_tuple(index, inMaterial);
            return true;
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<Material> MaterialComponent::get(int32 index)
        {
            R_ASSERT(index < materials.size());

            auto& tuple = materials[index];

            int32 id;
            ref_ptr<Material> material;

            std::tie(id, material) = tuple;

            return material;
        }

        //-------------------------------------------------------------------------
        const rex::ref_ptr<Material> MaterialComponent::get(int32 index) const
        {
            R_ASSERT(index < materials.size());

            auto& tuple = materials[index];

            int32 id;
            ref_ptr<Material> material;

            std::tie(id, material) = tuple;

            return material;
        }
    } // namespace ecs
} // namespace rex