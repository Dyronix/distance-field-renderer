#include "rex_io_pch.h"

#include "file_import.h"

#define ENABLE_IMPORT_LOGGING 0

#if ENABLE_IMPORT_LOGGING
#define FILE_IMPORT_TRACE R_TRACE
#define FILE_IMPORT_INFO R_INFO
#define FILE_IMPORT_WARN R_WARN
#define FILE_IMPORT_ERROR R_ERROR
#else
#define FILE_IMPORT_TRACE(...) UNUSED_PARAM(__VA_ARGS__)
#define FILE_IMPORT_INFO(...) UNUSED_PARAM(__VA_ARGS__)
#define FILE_IMPORT_WARN(...) UNUSED_PARAM(__VA_ARGS__)
#define FILE_IMPORT_ERROR(...) UNUSED_PARAM(__VA_ARGS__)
#endif

namespace rex
{
    // http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    std::istream& safe_get_line(std::istream& is, std::string& t)
    {
        t.clear();

        // The characters in the stream are read one-by-one using a std::streambuf.
        // That is faster than reading them one-by-one using the std::istream.
        // Code that uses streambuf this way must be guarded by a sentry object.
        // The sentry object performs various tasks,
        // such as thread synchronization and updating the stream state.

        std::istream::sentry se(is, true);
        std::streambuf* sb = is.rdbuf();

        if (se)
        {
            for (;;)
            {
                int c = sb->sbumpc();
                switch (c)
                {
                    case '\n': { return is;
                    }
                    case '\r':
                    {
                        if (sb->sgetc() == '\n')
                        {
                            sb->sbumpc();
                        }

                        return is;
                    }
                    case EOF:
                    {
                        // Also handle the case when the last line has no line ending
                        if (t.empty())
                        {
                            is.setstate(std::ios::eofbit);
                        }

                        return is;
                    }
                    default: { t += static_cast<char>(c);
                    }
                }
            }
        }

        return is;
    }

    //-------------------------------------------------------------------------
    rex::FileMemory read_from_disk(const std::string& path, ReadBinary readBinary, NullTerminate nullTerminate)
    {
        std::ios_base::openmode open_mode = std::ios::ate;
        if (readBinary)
        {
            open_mode = static_cast<std::ios_base::openmode>(std::ios::ate | std::ios::binary);
        }

        // std::ios::ate will put the "read cursor" at the end of the file.
        std::ifstream in_file(path, open_mode);

        std::unique_ptr<memory::byte> data;

        size_t file_size = 0;
        size_t null_terminatation_offset = nullTerminate ? 1 : 0;

        if (in_file.is_open())
        {
            FILE_IMPORT_INFO("Loading {0} into memory ...", path);

            file_size = (size_t)in_file.tellg();
            if (file_size == 0)
            {
                FILE_IMPORT_WARN("Empty file was given: {0} Skipping this one ... ", path);
                return {std::move(data), file_size};
            }

            // allocate 1 extra byte to set the termination char
            data.reset((memory::byte*)malloc(file_size + null_terminatation_offset));

            in_file.seekg(0);
            in_file.read((char*)data.get(), file_size);
            in_file.close();

            if (nullTerminate)
            {
                data.get()[file_size] = (memory::byte)'\0';
            }
        }
        else
        {
            FILE_IMPORT_WARN("Could not open file, maybe the file was not found? {0}", path);
        }

        FILE_IMPORT_TRACE("Loaded {0} bytes into memory", file_size);

        return {std::move(data), file_size};
    }

    //-------------------------------------------------------------------------
    rex::FileMemory read_from_disk(const std::string& path, std::function<bool(const std::string&)> parseFn, ReadBinary readBinary, NullTerminate nullTerminate)
    {
        std::ios_base::openmode open_mode = std::ios::in;
        if (readBinary)
        {
            open_mode = static_cast<std::ios_base::openmode>(std::ios::in | std::ios::binary);
        }

        std::ifstream in_file(path, open_mode);

        std::string content;
        std::string line;
        if (in_file.is_open())
        {
            while (in_file.peek() != -1)
            {
                safe_get_line(in_file, line);

                if (parseFn(line))
                {
                    content += line;
                }
            }

            in_file.close();
        }
        else
        {
            FILE_IMPORT_WARN("Could not open file, maybe the file was not found? {0}", path);
        }

        FILE_IMPORT_INFO("Loading {0} into memory ...", path);

        // NULL terminate string
        if (nullTerminate)
        {
            content += '\0';
        }

        std::unique_ptr<memory::byte> data((memory::byte*)malloc(content.size()));
        size_t file_size = content.size();

        strncpy_s((char*)data.get(), content.size(), content.c_str(), content.size());

        FILE_IMPORT_TRACE("Loaded {0} bytes into memory", file_size);

        return {std::move(data), file_size};
    }
}