#pragma once

namespace rex
{
    class Model;
}

namespace regina
{
    namespace model_importer
    {
        rex::ref_ptr<rex::Model> import(const rex::StringID& path);
    }
} // namespace regina