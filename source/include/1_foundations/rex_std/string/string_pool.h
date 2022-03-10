#pragma once

#include "string/string_ids.h"

#include <string>
#include <unordered_map>

namespace rex
{
    class StringEntry;
    class StringEntryID;

    namespace string_pool
    {
        void resolve(const StringEntryID& entryID, char** out, size_t& outSize);
        void resolve(const StringEntryID& entryID, std::string& out);

        const StringEntry* find(const StringEntryID& entryID);

        const StringEntryID* store(const ESID& name);
        const StringEntryID* store(const char* characters, size_t size);

        uint32 hash(const std::string& s);
    };
}