#pragma once

namespace rex
{
    class Model;

    namespace model_library
    {
        bool add(const ref_ptr<rex::Model>& model);
        bool add(const std::vector<ref_ptr<rex::Model>>& models);

        ref_ptr<rex::Model> get(const StringID& modelName);
        std::vector<ref_ptr<rex::Model>> get_all();

        void clear();
    };
}