#include "regina_pch.h"

#include "entry_point.h"
#include "regina.h"

#include "application_arguments.h"

//-----------------------------------------------------------------------
rex::CoreApplication* rex::create_application(const ApplicationArguments& arguments)
{
    rex::StringID window_width = arguments.get_argument_value("xres");
    rex::StringID window_height = arguments.get_argument_value("yres");
    rex::StringID fullscreen = arguments.get_argument_value("fullscreen");

    rex::ApplicationDescription description;

    description.name = "REX";
    description.window_width = !window_width.is_none() ? std::stoi(window_width.to_string()) : 1280;
    description.window_height = !window_height.is_none() ? std::stoi(window_height.to_string()) : 720;
    description.fullscreen = !fullscreen.is_none() ? std::stoi(fullscreen.to_string()) : false;

    return new regina::Application(description);
}

namespace regina
{
    //-------------------------------------------------------------------------
    Application::Application(const rex::ApplicationDescription& description)
        :rex::win32::Application(description)
    {

    }

    //-------------------------------------------------------------------------
    Application::~Application()
    {

    }
}