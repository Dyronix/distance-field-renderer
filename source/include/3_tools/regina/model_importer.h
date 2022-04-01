#pragma once

namespace rex
{
    class Model;
}

namespace regina
{
    namespace model_importer
    {
        rex::ref_ptr<rex::Model> import(const rex::StringID& path, const rex::StringID& name = rex::ESID::SID_None);
    }
} // namespace regina