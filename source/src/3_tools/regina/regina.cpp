#include "regina_pch.h"

#include "entry_point.h"
#include "regina.h"

//-----------------------------------------------------------------------
rex::CoreApplication* rex::create_application()
{
    return new regina::Application();
}

namespace regina
{
    //-------------------------------------------------------------------------
    Application::Application()
    {

    }

    //-------------------------------------------------------------------------
    Application::~Application()
    {

    }
}