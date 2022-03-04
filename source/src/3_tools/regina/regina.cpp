#include "regina_pch.h"

#include "entry_point.h"
#include "regina.h"

//-----------------------------------------------------------------------
rex::CoreApplication* rex::create_application(const ApplicationArguments& arguments)
{
    std::optional<std::string> window_width = arguments.get_argument_value("xres");
    std::optional<std::string> window_height = arguments.get_argument_value("yres");
    std::optional<std::string> fullscreen = arguments.get_argument_value("fullscreen");

    rex::ApplicationDescription description;

    description.window_width = window_width ? std::stoi(*window_width) : 1280;
    description.window_height = window_height ? std::stoi(*window_height) : 720;
    description.fullscreen = fullscreen ? std::stoi(*fullscreen) : false;

    return new regina::Application(description);
}

namespace regina
{
    //-------------------------------------------------------------------------
    Application::Application(const rex::ApplicationDescription& description)
        :CoreApplication(description)
    {

    }

    //-------------------------------------------------------------------------
    Application::~Application()
    {

    }
}