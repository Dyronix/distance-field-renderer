#pragma once

#include "core_application.h"

namespace regina
{
    class Application : public rex::CoreApplication
    {
    public:
        Application();
        ~Application() override;
    };
}