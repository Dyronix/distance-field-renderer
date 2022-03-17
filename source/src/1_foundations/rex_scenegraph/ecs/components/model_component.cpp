#include "rex_scenegraph_pch.h"

#include "ecs/components/model_component.h"

#include "model.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        ModelComponent::ModelComponent()
            : model(nullptr)
        {
        }
        //-------------------------------------------------------------------------
        ModelComponent::ModelComponent(const ModelComponent& other)
            : model(other.model)
        {
        }
        //-------------------------------------------------------------------------
        ModelComponent::ModelComponent(ModelComponent&& other) noexcept
            : model(std::exchange(other.model, {}))
        {

        }
        //-------------------------------------------------------------------------
        ModelComponent::ModelComponent(const ref_ptr<Model>& inModel)
            : model(inModel)
        {
        }

        //-------------------------------------------------------------------------
        rex::ecs::ModelComponent& ModelComponent::operator=(const ModelComponent& other)
        {
            this->model = other.model;

            return *this;
        }

        //-------------------------------------------------------------------------
        rex::ecs::ModelComponent& ModelComponent::operator=(ModelComponent&& other) noexcept
        {
            this->model = std::exchange(other.model, {});

            return *this;
        }

        //-------------------------------------------------------------------------
        ModelComponent::operator ref_ptr<Model>&()
        {
            return model;
        }
        //-------------------------------------------------------------------------
        ModelComponent::operator const ref_ptr<Model>&() const
        {
            return model;
        }

        //-------------------------------------------------------------------------
        bool ModelComponent::operator==(const ModelComponent& other) const
        {
            return model == other.model;
        }
        //-------------------------------------------------------------------------
        bool ModelComponent::operator!=(const ModelComponent& other) const
        {
            return !(*this == other);
        }
    } // namespace ecs
} // namespace rex