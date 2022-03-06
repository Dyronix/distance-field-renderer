#pragma once

#include "win32_application.h"

namespace regina
{
    class Application : public rex::win32::Application
    {
    public:
        Application(const rex::ApplicationDescription& description);
        ~Application() override;
    };
}