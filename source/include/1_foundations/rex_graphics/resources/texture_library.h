#pragma once

namespace rex
{
    class Texture;

    namespace texture_library
    {
        bool add(const ref_ptr<rex::Texture>& program);
        bool add(const std::vector<ref_ptr<rex::Texture>>& programs);

        ref_ptr<rex::Texture> get(const StringID& programName);

        void clear();
    };
}