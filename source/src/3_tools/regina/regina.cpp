#include "regina_pch.h"

#include "entry_point.h"

#include "regina.h"
#include "deferred_rendering_layer.h"
#include "distance_field_rendering_layer.h"
#include "lattice_rendering_layer.h"
#include "shortcut_layer.h"

#include "application_arguments.h"

#include "core_window.h"

namespace
{
    bool s_deferred = true;

    regina::DeferredRenderingLayerDescription s_deferred_description;
    regina::DistanceFieldRenderingLayerDescription s_distance_description;
} // namespace

//-----------------------------------------------------------------------
rex::CoreApplication* rex::create_application(const ApplicationArguments& arguments)
{
    rex::StringID window_width = arguments.get_argument_value("xres");
    rex::StringID window_height = arguments.get_argument_value("yres");
    rex::StringID fullscreen = arguments.get_argument_value("fullscreen");
    rex::StringID app_name = arguments.get_argument_value("name");
    rex::StringID profile_id = arguments.get_argument_value("profile");
    rex::StringID animate_scene = arguments.get_argument_value("animate");

    rex::StringID deferred = arguments.get_argument_value("deferred");
    rex::StringID mesh_type = arguments.get_argument_value("meshtype");
    rex::StringID max_iterations = arguments.get_argument_value("max_iterations");
    rex::StringID nr_lights = arguments.get_argument_value("nr_lights");
    rex::StringID lattice = arguments.get_argument_value("lattice");
    rex::StringID heatmap = arguments.get_argument_value("heatmap");
    rex::StringID resolution = arguments.get_argument_value("resolution");
    rex::StringID source_location = arguments.get_argument_value("source_location");

    rex::ApplicationDescription description;

    description.name = app_name.is_none() ? create_sid("REGINA") : app_name;
    description.profile_id = profile_id.is_none() ? create_sid("REGINA") : profile_id;
    description.window_width = !window_width.is_none() ? std::stoi(window_width.to_string()) : 1280;
    description.window_height = !window_height.is_none() ? std::stoi(window_height.to_string()) : 720;
    description.fullscreen = !fullscreen.is_none() ? std::stoi(fullscreen.to_string()) : false;

    s_deferred_description.mesh_type = !mesh_type.is_none() ? std::stoi(mesh_type.to_string()) : 0;
    s_deferred_description.nr_lights = !nr_lights.is_none() ? std::stoi(nr_lights.to_string()) : 32;
    s_deferred_description.resolution = !resolution.is_none() ? std::stoi(resolution.to_string()) : -1;
    s_deferred_description.use_lattice = !lattice.is_none() ? std::stoi(lattice.to_string()) : false;
    s_deferred_description.source_content_location = !source_location.is_none() ? source_location : ESID::SID_None;
    s_deferred_description.animate = !animate_scene.is_none() ? std::stoi(animate_scene.to_string()) : false;

    s_distance_description.max_iterations = !max_iterations.is_none() ? std::stoi(max_iterations.to_string()) : 150;
    s_distance_description.nr_lights = !nr_lights.is_none() ? std::stoi(nr_lights.to_string()) : 32;
    s_distance_description.volume_type = !mesh_type.is_none() ? std::stoi(mesh_type.to_string()) : 0;
    s_distance_description.use_heatmap = !heatmap.is_none() ? std::stoi(heatmap.to_string()) : false;
    s_distance_description.resolution = !resolution.is_none() ? std::stoi(resolution.to_string()) : -1;
    s_distance_description.use_lattice = !lattice.is_none() ? std::stoi(lattice.to_string()) : false;
    s_distance_description.source_content_location = !source_location.is_none() ? source_location : ESID::SID_None;
    s_distance_description.animate = !animate_scene.is_none() ? std::stoi(animate_scene.to_string()) : false;

    s_deferred = !deferred.is_none() ? std::stoi(deferred.to_string()) : 0;

    return new regina::Application(description);
}

namespace regina
{
    //-------------------------------------------------------------------------
    Application::Application(const rex::ApplicationDescription& description)
        : rex::win32::Application(description)
        , m_deferred_layer(nullptr)
        , m_distance_field_layer(nullptr)
        , m_shotcut_layer(nullptr)
    {
    }

    //-------------------------------------------------------------------------
    Application::~Application()
    {
    }

    //-------------------------------------------------------------------------
    void Application::on_app_initialize()
    {
        R_PROFILE_FUNCTION();

        const rex::CoreWindow* window = get_window();

        if (!s_deferred)
        {
            //m_distance_field_layer = push_back_layer(std::make_unique<DistanceFieldRenderingLayer>(window, s_distance_description));
            m_distance_field_layer = push_back_layer(std::make_unique<LatticeRenderingLayer>(window));
        }
        else
        {
            m_deferred_layer = push_back_layer(std::make_unique<DeferredRenderingLayer>(window, s_deferred_description));
        }
        m_shotcut_layer = push_back_layer(std::make_unique<ShortcutLayer>());
    }

    static int32 frame_counter = 0;

    //-------------------------------------------------------------------------
    void Application::on_app_update(const rex::FrameInfo& /*info*/)
    {
        ++frame_counter;

        //if (frame_counter > 500)
        //{
        //    quit();
        //}
    }
}