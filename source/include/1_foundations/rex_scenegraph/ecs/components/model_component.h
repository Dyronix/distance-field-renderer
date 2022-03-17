#pragma once

namespace rex
{
    class Model;

    namespace ecs
    {
        struct ModelComponent
        {
            ModelComponent();
            ModelComponent(const ModelComponent& other);
            ModelComponent(ModelComponent&& other) noexcept;
            ModelComponent(const ref_ptr<Model>& inModel);

            ModelComponent& operator=(const ModelComponent& other);
            ModelComponent& operator=(ModelComponent&& other) noexcept;

            operator ref_ptr<Model>&();
            operator const ref_ptr<Model>&() const;

            bool operator==(const ModelComponent& other) const;
            bool operator!=(const ModelComponent& other) const;

            ref_ptr<Model> model;
        };
    }
}