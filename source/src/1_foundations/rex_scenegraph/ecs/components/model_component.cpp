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
        ModelComponent::ModelComponent(const ref_ptr<Model>& inModel)
            : model(inModel)
        {
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
    }
}