#pragma once

#include "string/stringid.h"

#include <vector>
#include <unordered_map>

namespace sbt
{
    class Texture;

    namespace texture_library
    {
        bool add(const ref_ptr<sbt::Texture>& program);
        bool add(const std::vector<ref_ptr<sbt::Texture>>& programs);

        ref_ptr<sbt::Texture> get(const StringID& programName);

        void clear();
    };
}