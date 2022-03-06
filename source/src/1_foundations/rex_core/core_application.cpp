#include "rex_core_pch.h"

#include "core_application.h"

namespace rex
{
    //-------------------------------------------------------------------------
    CoreApplication* CoreApplication::s_instance = nullptr;

    //-------------------------------------------------------------------------
    CoreApplication::CoreApplication(const ApplicationDescription& description)
        :m_description(description)
    {
        R_INFO("Creating application: {0}", description.name);

        R_ASSERT_X(CoreApplication::s_instance == nullptr, "There can only be one application at the time");
        CoreApplication::s_instance = this;
    }

    //-------------------------------------------------------------------------
    CoreApplication::~CoreApplication()
    {
        R_INFO("Destroying application");
    }

    //-------------------------------------------------------------------------
    int32 CoreApplication::run()
    {
        if (!initialize()) return EXIT_FAILURE;
        if (!shutdown()) return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    //-------------------------------------------------------------------------
    const rex::ApplicationDescription& CoreApplication::get_application_description() const
    {
        return m_description;
    }

    //-------------------------------------------------------------------------
    bool CoreApplication::initialize()
    {
        return platform_initialize();
    }
    //-------------------------------------------------------------------------
    bool CoreApplication::shutdown()
    {
        return platform_shutdown();
    }
}