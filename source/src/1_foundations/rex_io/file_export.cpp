#include "rex_io_pch.h"

#include "file_export.h"

#define ENABLE_EXPORT_LOGGING 0

#if ENABLE_EXPORT_LOGGING
#define FILE_EXPORT_TRACE S_TRACE
#define FILE_EXPORT_INFO S_INFO
#define FILE_EXPORT_WARN S_WARN
#define FILE_EXPORT_ERROR S_ERROR
#else
#define FILE_EXPORT_TRACE(...) UNUSED_PARAM(__VA_ARGS__)
#define FILE_EXPORT_INFO(...) UNUSED_PARAM(__VA_ARGS__)
#define FILE_EXPORT_WARN(...) UNUSED_PARAM(__VA_ARGS__)
#define FILE_EXPORT_ERROR(...) UNUSED_PARAM(__VA_ARGS__)
#endif

namespace rex
{
    //-------------------------------------------------------------------------
    bool write_to_disk(const std::string& path, const FileMemory& fileData, WriteBinary writeBinary)
    {
        std::ios_base::openmode open_mode = std::ios::out | std::ios::trunc;
        if (writeBinary)
        {
            open_mode = std::ios::out | std::ios::trunc | std::ios::binary;
        }

        std::ofstream out_file(path, open_mode);

        if (out_file.is_open())
        {
            FILE_EXPORT_INFO("Writing {0} to disk", path);

            out_file.write(fileData.get_data_as<char>(), fileData.get_size());
            out_file.close();
        }
        else
        {
            FILE_EXPORT_WARN("Could not open file, maybe the file was not found? {0}", path);
            return false;
        }

        return true;
    }
}