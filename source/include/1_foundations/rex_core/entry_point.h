#pragma once

#include "types.h"
#include "core_application.h"

#include <iostream>

//--------------------------------------------------------------------------------------------
extern rex::CoreApplication* rex::create_application(const ApplicationArguments& arguments);

//--------------------------------------------------------------------------------------------
int32 runProgram(int32 argc, char** argv)
{
    std::cout << "Starting REX" << std::endl;

    rex::ApplicationArguments application_arguments(argc, argv);

    rex::CoreApplication* application = rex::create_application(application_arguments);

    int32 result = application->run();

    delete application;

    std::cout << "Ending SCRIBIT" << std::endl;

    return result;
}

#if defined REX_PLATFORM_WINDOWS

//--------------------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
    int32 result = runProgram(argc, argv);

    return result;
}

#elif defined REX_PLATFORM_MACOS

//--------------------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
    int32 result = runProgram(argc, argv);

    return result;
}

#elif defined REX_PLATFORM_LINUX

//--------------------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
    int32 result = runProgram(argc, argv);

    return result;
}

#endif