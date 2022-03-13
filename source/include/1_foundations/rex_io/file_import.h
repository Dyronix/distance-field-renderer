#pragma once

#include "yes_no_enum.h"
#include "file_memory.h"

namespace rex
{
    using ReadBinary = YesNoEnum;
    using NullTerminate = YesNoEnum;

    FileMemory read_from_disk(const std::string& path, ReadBinary readBinary = YesNoEnum::NO, NullTerminate nullTerminate = NullTerminate::YES);
    FileMemory read_from_disk(const std::string& path, std::function<bool(const std::string&)> parseFn, ReadBinary readBinary = YesNoEnum::NO, NullTerminate nullTerminate = NullTerminate::YES);
}