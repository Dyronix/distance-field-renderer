#pragma once

namespace rex
{
    class CoreApplication
    {
    public:
        CoreApplication();
        virtual ~CoreApplication();

        int32 run(int32 argc, char** argv);
    };

    // This will be implemented by the CLIENT
    CoreApplication* create_application();
}