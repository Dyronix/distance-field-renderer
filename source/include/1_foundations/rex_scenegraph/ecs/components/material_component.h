#pragma once

namespace rex
{
    class Material;

    namespace ecs
    {
        struct MaterialComponent
        {
            MaterialComponent();
            MaterialComponent(const MaterialComponent& other);
            MaterialComponent(MaterialComponent&& other) noexcept;
            MaterialComponent(const ref_ptr<Material>& inMaterial);
            MaterialComponent(const std::vector<ref_ptr<Material>>& inMaterials);

            MaterialComponent& operator=(const MaterialComponent& other);
            MaterialComponent& operator=(MaterialComponent&& other) noexcept;

            operator std::vector<std::tuple<int32, ref_ptr<Material>>>&();
            operator const std::vector<std::tuple<int32, ref_ptr<Material>>>&() const;

            bool operator==(const MaterialComponent& other) const;
            bool operator!=(const MaterialComponent& other) const;

            bool add(const ref_ptr<Material>& inMaterial);
            bool set(int32 index, const ref_ptr<Material>& inMaterial);

            ref_ptr<Material> get(int32 index);
            const ref_ptr<Material> get(int32 index) const;

            std::vector<std::tuple<int32, ref_ptr<Material>>> materials;
        };
    }
}