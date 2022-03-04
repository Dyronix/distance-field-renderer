#pragma once

namespace rex
{
    class ApplicationArguments;

    struct ApplicationDescription
    {
        int32 window_width;
        int32 window_height;

        bool fullscreen;
    };

    class CoreApplication
    {
    public:
        CoreApplication(const ApplicationDescription& description);
        virtual ~CoreApplication();

        int32 run();
    };

    // This will be implemented by the CLIENT
    CoreApplication* create_application(const ApplicationArguments& arguments);
}