#include "rex_core_pch.h"

#include "application_arguments.h"
#include "application_arguments_parser.h"

#include "string/string_operations.h"

namespace rex
{
    //-------------------------------------------------------------------------
    ApplicationArguments::ApplicationArguments(int32 inArgc, char** inArgv)
        : m_argc(inArgc)
        , m_argv(inArgv)
    {
        m_arguments.reserve(inArgc);

        // First argument is our working directory
        R_TODO("Capture the working directory when parsing application arguments");
        
        R_INFO("[Application Argument] idx: 0, value: {0}", inArgv[0]);

        for (int i = 1; i < inArgc; ++i)
        {
            R_INFO("[Application Argument] idx: {0}, value: {1}", i, inArgv[i]);

            ApplicationArgument argument = application_argument_parser::parse(inArgv[i]);

            m_arguments.insert(std::make_pair(create_sid(argument.key), create_sid(argument.value)));
        }
    }
    //-------------------------------------------------------------------------
    ApplicationArguments::~ApplicationArguments() = default;

    //-------------------------------------------------------------------------
    bool ApplicationArguments::has_arguments() const
    {
        return !m_arguments.empty();
    }
    //-------------------------------------------------------------------------
    bool ApplicationArguments::has_argument(const StringID& arg) const
    {
        return m_arguments.find(arg) != std::cend(m_arguments);
    }

    //-------------------------------------------------------------------------
    StringID ApplicationArguments::get_argument_value(const StringID& arg) const
    {
        auto it = m_arguments.find(arg);
        if (it == std::cend(m_arguments))
        {
            return ESID::SID_None;
        }

        return it->second;
    }

    //-------------------------------------------------------------------------
    int32 ApplicationArguments::get_argument_count() const
    {
        return m_argc;
    }
    //-------------------------------------------------------------------------
    char** ApplicationArguments::get_argument_values() const
    {
        return m_argv;
    }
}