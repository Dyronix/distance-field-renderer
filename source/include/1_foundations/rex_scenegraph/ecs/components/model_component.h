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
            ModelComponent(const ref_ptr<Model>& inModel);

            operator ref_ptr<Model>&();
            operator const ref_ptr<Model>&() const;

            bool operator==(const ModelComponent& other) const;
            bool operator!=(const ModelComponent& other) const;

            ref_ptr<Model> model;
        };
    }
}