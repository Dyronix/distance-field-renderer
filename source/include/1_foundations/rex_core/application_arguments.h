#pragma once

namespace rex
{
    class ApplicationArguments
    {
    public:
        ApplicationArguments(int32 argc, char** argv);
        ~ApplicationArguments();

        bool has_arguments() const;
        bool has_argument(const StringID& arg) const;

        StringID get_argument_value(const StringID& arg) const;

        int32 get_argument_count() const;
        char** get_argument_values() const;

    private:
        using Arguments = std::unordered_map<StringID, StringID>;

        Arguments m_arguments;

        int32 m_argc;
        char** m_argv;
    };
}