#pragma once

#include "file_import.h"
#include "file_memory.h"

#include "blobreader.h"

namespace rex
{
    namespace token_parser_helpers
    {
        bool        try_parse_double(const char* s, const char* s_end, double* out);

        float       parse_float(const char** token);

        void        parse_float2(const char** token, float* outX, float* outY);
        void        parse_float3(const char** token, float* outX, float* outY, float* outZ);

        int         parse_int(const char** token);

        std::string parse_string(const char** token);

        //-------------------------------------------------------------------------
        template <typename T>
        bool        is_space(const T& x)
        {
            return (((x) == ' ') || ((x) == '\t'));
        }

        //-------------------------------------------------------------------------
        template <typename T>
        bool        is_digit(const T& x)
        {
            return (static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10));
        }
    }

    template <typename T>
    class TokenParser
    {
    public:
        virtual ~TokenParser() = default;

        T parse(const FileMemory& input);

    protected:
        TokenParser() = default;

        virtual bool on_parse_line(T& parsedData, const std::string& line, const char* currToken) = 0;

    private:
        bool parse_line(T& parsedData, const std::string& line);
    };

    //-------------------------------------------------------------------------
    template <typename T>
    T TokenParser<T>::parse(const FileMemory& input)
    {
        T parsed_data = {};

        int32 total_size = 0;
        while (total_size < input.get_size())
        {
            std::string line;

            int8 character = 0;
            do
            {
                character = memory::reader::read<int8>(input, total_size);

                line += character;

                total_size += sizeof(character);
            } while (character != '\n' && character != '\r');

            parse_line(parsed_data, line);
        }

        return parsed_data;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    bool TokenParser<T>::parse_line(T& parsedData, const std::string& line)
    {
        std::string in_line = line;

        // Trim newline '\r\n' or '\n'
        if (in_line.size() > 0)
        {
            if (in_line[in_line.size() - 1] == '\n')
            {
                in_line.erase(in_line.size() - 1);
            }
        }
        if (in_line.size() > 0)
        {
            if (in_line[in_line.size() - 1] == '\r')
            {
                in_line.erase(in_line.size() - 1);
            }
        }

        // Skip if empty line.
        if (in_line.empty())
        {
            return false;
        }

        // Skip leading space.
        const char* token = in_line.c_str();

        token += strspn(token, " \t");

        R_ASSERT(token);

        if (token[0] == '\0') return false;  // empty line
        if (token[0] == '#')  return false;  // comment line

        return on_parse_line(parsedData, in_line, token);
    }
}