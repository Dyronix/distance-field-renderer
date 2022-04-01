#include "rex_graphics_pch.h"

#include "model_library.h"
#include "model.h"

namespace rex
{
    namespace model_library
    {
        std::unordered_map<StringID, ref_ptr<Model>> g_models;

        //-------------------------------------------------------------------------
        bool add(const ref_ptr<rex::Model>& model)
        {
            auto it = g_models.find(model->get_name());
            if (it != std::cend(g_models))
            {
                return false;
            }

            g_models.insert(std::make_pair(model->get_name(), model));
            return true;
        }
        //-------------------------------------------------------------------------
        bool add(const std::vector<ref_ptr<rex::Model>>& models)
        {
            bool result = true;
            for (auto& p : models)
            {
                result &= add(p);
            }

            return result;
        }

        //-------------------------------------------------------------------------
        ref_ptr<rex::Model> get(const StringID& modelName)
        {
            auto it = g_models.find(modelName);

            return it != std::cend(g_models) ? it->second : nullptr;
        }

        //-------------------------------------------------------------------------
        std::vector<rex::ref_ptr<rex::Model>> getAll()
        {
            std::vector<ref_ptr<Model>> models;
            std::transform(std::cbegin(g_models), std::cend(g_models), std::back_inserter(models), [](const auto& pair)
                           {
                               return pair.second;
                           });

            return models;
        }

        //-------------------------------------------------------------------------
        void clear()
        {
            g_models.clear();
        }
    }
}