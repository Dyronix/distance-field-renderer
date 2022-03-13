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
            MaterialComponent(const ref_ptr<Material>& inMaterial);

            operator ref_ptr<Material>&();
            operator const ref_ptr<Material>&() const;

            bool operator==(const MaterialComponent& other) const;
            bool operator!=(const MaterialComponent& other) const;

            ref_ptr<Material> material;
        };
    }
}