#pragma once

#include "types.h"
#include "core_application.h"
#include "application_arguments.h"

#include "rex_debug.h"

#include <iostream>

//--------------------------------------------------------------------------------------------
extern rex::CoreApplication* rex::create_application(const ApplicationArguments& arguments);

//--------------------------------------------------------------------------------------------
int32 run_program(int32 argc, char** argv)
{
    std::cout << "Starting REX" << std::endl;

#ifdef REX_DEBUG
    rex::logging::create();

#if REX_ENABLE_PROFILE
    R_PROFILE_ENABLE();
#else
    R_PROFILE_DISABLE();
#endif
#else
    R_PROFILE_DISABLE();
#endif

    rex::ApplicationArguments application_arguments(argc, argv);

    rex::CoreApplication* application = rex::create_application(application_arguments);

    int32 result = application->run();

    delete application;

#ifdef REX_DEBUG
#if REX_ENABLE_PROFILE
    R_PROFILE_DISABLE();
#endif

    rex::logging::destroy();
#endif

    std::cout << "Ending REX" << std::endl;

    return result;
}

#if defined REX_PLATFORM_WINDOWS

//--------------------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
    int32 result = run_program(argc, argv);

    return result;
}

#elif defined REX_PLATFORM_MACOS

//--------------------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
    int32 result = run_program(argc, argv);

    return result;
}

#elif defined REX_PLATFORM_LINUX

//--------------------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
    int32 result = run_program(argc, argv);

    return result;
}

#endif