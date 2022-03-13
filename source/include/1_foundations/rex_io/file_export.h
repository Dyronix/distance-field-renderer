#pragma once

#include "yes_no_enum.h"
#include "file_memory.h"

namespace rex
{
    using WriteBinary = YesNoEnum;

    bool write_to_disk(const std::string& path, const FileMemory& fileData, WriteBinary writeBinary = WriteBinary::NO);
}