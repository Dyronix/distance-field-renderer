#include "regina_pch.h"

#include "entry_point.h"

#include "regina.h"
#include "geometry_rendering_layer.h"
#include "volume_rendering_layer.h"
#include "lattice_rendering_layer.h"
#include "shortcut_layer.h"

#include "application_arguments.h"

#include "core_window.h"

namespace
{
    namespace tags
    {
        const rex::StringID WINDOW_WIDTH            = "xres"_sid;
        const rex::StringID WINDOW_HEIGHT           = "yres"_sid;
        const rex::StringID FULLSCREEN              = "fullscreen"_sid;
        const rex::StringID APPLICATION_NAME        = "app_name"_sid;        

        const rex::StringID ANIMATE_SCENE           = "animate_scene"_sid;
        const rex::StringID LATTIFIED               = "lattified"_sid;
        const rex::StringID SHAPE_TYPE              = "shape_type"_sid;
        const rex::StringID NR_LIGHTS               = "nr_lights"_sid;
        const rex::StringID CONTENT_LOCATION        = "content_location"_sid;

        const rex::StringID MAX_ITERATIONS          = "max_iterations"_sid;
        const rex::StringID MAX_MARCHING_DISTANCE   = "max_marching_distance"_sid;
        const rex::StringID MIN_MARCHING_DISTANCE   = "min_marching_distance"_sid;
        const rex::StringID HEATMAP                 = "heatmap"_sid;

        const rex::StringID GEOMETRY_TYPE           = "geometry_type"_sid;
    }

    bool s_geometry_layer;

    regina::GeometryRenderingLayerDescription s_deferred_description;
    regina::VolumeRenderingLayerDescription s_distance_description;

    enum class ApplicationArgumentParserType
    {
        NONE,
        GEOMETRY,
        VOLUME
    };

    class ApplicationArgumentsParser
    {
    public:
        using FullScreen = rex::YesNoEnum;
        using AnimateScene = rex::YesNoEnum;
        using Lattified = rex::YesNoEnum;

        //-----------------------------------------------------------------------
        ApplicationArgumentsParser(const rex::ApplicationArguments& applicationArguments)
            :m_window_width(1280)
            ,m_window_height(720)
            ,m_fullscreen(FullScreen::NO)
            ,m_animate_scene(AnimateScene::NO)
            ,m_lattified(Lattified::NO)
            ,m_application_name(rex::ESID::SID_None)
            ,m_content_location(rex::ESID::SID_None)
            ,m_shape_type(0)
            ,m_nr_lights(32)
        {
            m_window_width = load_window_width(applicationArguments);
            m_window_height = load_window_height(applicationArguments);

            m_fullscreen = load_fullscreen(applicationArguments);
            m_animate_scene = load_animate_scene(applicationArguments);
            m_lattified = load_lattified(applicationArguments);
            m_application_name = load_application_name(applicationArguments);
            m_content_location = load_content_location(applicationArguments);
            m_shape_type = load_shape_type(applicationArguments);
            m_nr_lights = load_number_of_lights(applicationArguments);
        }   

        //-----------------------------------------------------------------------
        int32 get_window_width() const
        {
            return m_window_width;
        }
        //-----------------------------------------------------------------------
        int32 get_window_height() const
        {
            return m_window_height;
        }

        //-----------------------------------------------------------------------
        bool fullscreen() const 
        {
            return m_fullscreen == FullScreen::YES;
        }
        //-----------------------------------------------------------------------
        bool animate_scene() const
        {
            return m_animate_scene == AnimateScene::YES;
        }
        //-----------------------------------------------------------------------
        bool lattified() const
        {
            return m_lattified == Lattified::YES;
        }

        //-----------------------------------------------------------------------
        rex::StringID get_application_name() const
        {
            return m_application_name;
        }
        //-----------------------------------------------------------------------
        rex::StringID get_content_location() const
        {
            return m_content_location;
        }

        //-----------------------------------------------------------------------
        virtual rex::StringID get_profile_id() const = 0;

        //-----------------------------------------------------------------------
        int32 get_shape_type() const
        {
            return m_shape_type;
        }
        //-----------------------------------------------------------------------
        int32 get_number_of_lights() const
        {
            return m_nr_lights;
        }

        //-----------------------------------------------------------------------
        virtual ApplicationArgumentParserType get_parser_type() const = 0;

    private:
        //-----------------------------------------------------------------------
        int32 load_window_width(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_window_width = applicationArguments.get_argument_value(tags::WINDOW_WIDTH);
            
            return !sid_window_width.is_none() ? std::stoi(sid_window_width.to_string()) : 1280;
        }        
        //-----------------------------------------------------------------------
        int32 load_window_height(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_window_height = applicationArguments.get_argument_value(tags::WINDOW_HEIGHT);
            
            return !sid_window_height.is_none() ? std::stoi(sid_window_height.to_string()) : 720;
        }

        //-----------------------------------------------------------------------
        FullScreen load_fullscreen(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_fullscreen = applicationArguments.get_argument_value(tags::FULLSCREEN);
            bool fullscreen = !sid_fullscreen.is_none() ? std::stoi(sid_fullscreen.to_string()) : false;

            return fullscreen ? FullScreen::YES : FullScreen::NO;
        }
        //-----------------------------------------------------------------------
        AnimateScene load_animate_scene(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_animate_scene = applicationArguments.get_argument_value(tags::ANIMATE_SCENE);
            bool animate_scene = !sid_animate_scene.is_none() ? std::stoi(sid_animate_scene.to_string()) : false;

            return animate_scene ? AnimateScene::YES : AnimateScene::NO;
        }
        //-----------------------------------------------------------------------
        Lattified load_lattified(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_lattified = applicationArguments.get_argument_value(tags::LATTIFIED);
            bool lattified = !sid_lattified.is_none() ? std::stoi(sid_lattified.to_string()) : false;

            return lattified ? Lattified::YES : Lattified::NO;
        }
        
        //-----------------------------------------------------------------------
        rex::StringID load_application_name(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID application_name = applicationArguments.get_argument_value(tags::APPLICATION_NAME);

            return !application_name.is_none() ? application_name : "regina"_sid;
        }        
        //-----------------------------------------------------------------------
        rex::StringID load_content_location(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID content_location = applicationArguments.get_argument_value(tags::CONTENT_LOCATION);

            return !content_location.is_none() ? content_location : "content"_sid;
        }

        //-----------------------------------------------------------------------
        int32 load_shape_type(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_shape_type = applicationArguments.get_argument_value(tags::SHAPE_TYPE);
            
            return !sid_shape_type.is_none() ? std::stoi(sid_shape_type.to_string()) : 0;
        }        
        //-----------------------------------------------------------------------
        int32 load_number_of_lights(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_number_of_lights = applicationArguments.get_argument_value(tags::NR_LIGHTS);
            
            return !sid_number_of_lights.is_none() ? std::stoi(sid_number_of_lights.to_string()) : 32;
        }

        FullScreen m_fullscreen;
        AnimateScene m_animate_scene;
        Lattified m_lattified;

        rex::StringID m_application_name;
        rex::StringID m_content_location;

        int32 m_window_width;
        int32 m_window_height;
        int32 m_shape_type;
        int32 m_nr_lights;
    };

    class VolumeApplicationArgumentParser : public ApplicationArgumentsParser
    {
    public:
        using GenerateHeatmap = rex::YesNoEnum;

        //-----------------------------------------------------------------------
        VolumeApplicationArgumentParser(const rex::ApplicationArguments& applicationArguments)
            :ApplicationArgumentsParser(applicationArguments)
            ,m_max_iterations(100)
            ,m_max_marching_distance(1000.0f)
            ,m_min_marching_distance(0.01f)
            ,m_generate_heatmap(GenerateHeatmap::NO)
        {
            m_max_iterations = load_max_iterations(applicationArguments);
            m_max_marching_distance = load_max_marching_distance(applicationArguments);
            m_min_marching_distance = load_min_marching_distance(applicationArguments);
            m_generate_heatmap = load_generate_heatmap(applicationArguments);
        }

        //-----------------------------------------------------------------------
        int32 get_max_iterations() const
        {
            return m_max_iterations;
        }

        //-----------------------------------------------------------------------
        float get_max_marching_distance() const
        {
            return m_max_marching_distance;
        }
        //-----------------------------------------------------------------------
        float get_min_marching_distance() const
        {
            return m_min_marching_distance;
        }

        //-----------------------------------------------------------------------
        bool generate_heatmap() const
        {
            return m_generate_heatmap;
        }

        //-----------------------------------------------------------------------
        rex::StringID get_profile_id() const override 
        {
            std::stringstream stream;

            stream << get_application_name().to_string() ;
            stream << "_";
            stream << animate_scene() ? "dynamic_scene" : "static_scene"; 
            stream << "_";
            stream << "shape_type_" << get_shape_type();
            stream << "_";
            stream << lattified() ? "lattified_geometry" : "regular_geometry";
            stream << "_";
            stream << "nr_lights_" << get_number_of_lights();
            stream << "_";
            stream << "max_iterations_" << get_max_iterations();
            stream << "_";
            stream << "max_marching_distance_" << static_cast<int32>(get_max_marching_distance());
            stream << "_";
            stream << "min_marching_distance_" << static_cast<int32>(get_min_marching_distance());
            stream << "_";
            stream << generate_heatmap() ? "heatmap" : "no_heatmap";
            stream << "_";

            if(fullscreen())
            {
                stream << "fullscreen";
                stream << "_";
            }
            else
            {
                stream << "xres_" << static_cast<int32>(get_window_width());
                stream << "_";
                stream << "yres_" << static_cast<int32>(get_window_height());
                stream << "_";
            }

            stream << "volume";
            
            return rex::create_sid(stream.str());
        }

        //-----------------------------------------------------------------------
        ApplicationArgumentParserType get_parser_type() const override
        {
            return ApplicationArgumentParserType::VOLUME;
        }

    private:
        //-----------------------------------------------------------------------
        int32 load_max_iterations(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_max_iterations = applicationArguments.get_argument_value(tags::MAX_ITERATIONS);
            
            return !sid_max_iterations.is_none() ? std::stoi(sid_max_iterations.to_string()) : 100;
        }
        //-----------------------------------------------------------------------
        float load_max_marching_distance(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_max_marching_distance = applicationArguments.get_argument_value(tags::MAX_MARCHING_DISTANCE);
            
            return !sid_max_marching_distance.is_none() ? std::stof(sid_max_marching_distance.to_string()) : 1000.0f;
        }
        //-----------------------------------------------------------------------
        float load_min_marching_distance(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_max_marching_distance = applicationArguments.get_argument_value(tags::MIN_MARCHING_DISTANCE);
            
            return !sid_max_marching_distance.is_none() ? std::stof(sid_max_marching_distance.to_string()) : 0.01f;
        }
        //-----------------------------------------------------------------------
        GenerateHeatmap load_generate_heatmap(const rex::ApplicationArguments& applicationArguments) const
        {
            rex::StringID sid_heatmap = applicationArguments.get_argument_value(tags::HEATMAP);
            bool generate_heatmap = !sid_heatmap.is_none() ? std::stoi(sid_heatmap.to_string()) : false;

            return generate_heatmap ? GenerateHeatmap::YES : GenerateHeatmap::NO;
        }

        int32 m_max_iterations;

        float m_max_marching_distance;
        float m_min_marching_distance;

        GenerateHeatmap m_generate_heatmap;
    };

    class GeometryApplicationArgumentParser : public ApplicationArgumentsParser
    {
    public:
        //-----------------------------------------------------------------------
        GeometryApplicationArgumentParser (const rex::ApplicationArguments& applicationArguments)
            :ApplicationArgumentsParser(applicationArguments)
        {

        }

        //-----------------------------------------------------------------------
        rex::StringID get_profile_id() const override 
        {
            std::stringstream stream;

            stream << get_application_name().to_string() ;
            stream << "_";
            stream << animate_scene() ? "dynamic_scene" : "static_scene"; 
            stream << "_";
            stream << "shape_type_" << get_shape_type();
            stream << "_";
            stream << lattified() ? "lattified_geometry" : "regular_geometry";
            stream << "_";
            stream << "nr_lights_" << get_number_of_lights();
            stream << "_";

            if(fullscreen())
            {
                stream << "fullscreen";
                stream << "_";
            }
            else
            {
                stream << "xres_" << static_cast<int32>(get_window_width());
                stream << "_";
                stream << "yres_" << static_cast<int32>(get_window_height());
                stream << "_";
            }

            stream << "geometry";

            return rex::create_sid(stream.str());
        }

        //-----------------------------------------------------------------------
        ApplicationArgumentParserType get_parser_type() const override
        {
            return ApplicationArgumentParserType::GEOMETRY;
        }
    };
} // namespace

//-----------------------------------------------------------------------
bool is_geometry_layer(const rex::ApplicationArguments& applicationArguments)
{
    rex::StringID sid_deferred = applicationArguments.get_argument_value(tags::GEOMETRY_TYPE);
    int32 deferred = !sid_deferred.is_none() ? std::stoi(sid_deferred.to_string()) : 0;
    return deferred == 1;
}
//-----------------------------------------------------------------------
bool is_volume_layer(const rex::ApplicationArguments& applicationArguments)
{
    rex::StringID sid_deferred = applicationArguments.get_argument_value(tags::GEOMETRY_TYPE);
    int32 deferred = !sid_deferred.is_none() ? std::stoi(sid_deferred.to_string()) : 0;
    return deferred == 0;
}

//-----------------------------------------------------------------------
regina::GeometryRenderingLayerDescription make_geometry_rendering_layer_description(const ApplicationArgumentsParser& parser)
{
    R_ASSERT(parser.get_parser_type() == ApplicationArgumentParserType::GEOMETRY);

    const GeometryApplicationArgumentParser& geometry_parser = static_cast<const GeometryApplicationArgumentParser&>(parser);

    regina::GeometryRenderingLayerDescription description;

    description.mesh_type = geometry_parser.get_shape_type();
    description.nr_lights = geometry_parser.get_number_of_lights();
    description.use_lattice = geometry_parser.lattified();
    description.source_content_location = geometry_parser.get_content_location();
    description.animate = geometry_parser.animate_scene();

    return description;
}
//-----------------------------------------------------------------------
regina::VolumeRenderingLayerDescription make_volume_rendering_layer_description(const ApplicationArgumentsParser& parser)
{
    R_ASSERT(parser.get_parser_type() == ApplicationArgumentParserType::VOLUME);

    const VolumeApplicationArgumentParser& volume_parser = static_cast<const VolumeApplicationArgumentParser&>(parser);

    regina::VolumeRenderingLayerDescription description;

    description.max_iterations = volume_parser.get_max_iterations();
    description.nr_lights = volume_parser.get_number_of_lights();
    description.volume_type = volume_parser.get_shape_type();
    description.use_heatmap = volume_parser.generate_heatmap();
    description.use_lattice = volume_parser.lattified();
    description.source_content_location = volume_parser.get_content_location();
    description.animate = volume_parser.animate_scene();

    return description;
}

//-----------------------------------------------------------------------
rex::CoreApplication* rex::create_application(const ApplicationArguments& arguments)
{
    rex::ApplicationDescription description;
    
    if(is_geometry_layer(arguments))
    {
        GeometryApplicationArgumentParser parser = GeometryApplicationArgumentParser(arguments);

        description.name = parser.get_application_name();
        description.profile_id = parser.get_profile_id();
        description.window_width = parser.get_window_width();
        description.window_height = parser.get_window_height();
        description.fullscreen = parser.fullscreen();

        s_deferred_description = make_geometry_rendering_layer_description(parser);
        s_geometry_layer = true;
    }
    else
    {
        VolumeApplicationArgumentParser parser = VolumeApplicationArgumentParser(arguments);

        description.name = parser.get_application_name();
        description.profile_id = parser.get_profile_id();
        description.window_width = parser.get_window_width();
        description.window_height = parser.get_window_height();
        description.fullscreen = parser.fullscreen();

        s_distance_description = make_volume_rendering_layer_description(parser);
        s_geometry_layer = false;
    }

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

        if (s_geometry_layer)
        {
            m_distance_field_layer = push_back_layer(std::make_unique<VolumeRenderingLayer>(window, s_distance_description));
        }
        else
        {
            m_deferred_layer = push_back_layer(std::make_unique<GeometryRenderingLayer>(window, s_deferred_description));
        }

        m_shotcut_layer = push_back_layer(std::make_unique<ShortcutLayer>());
    }

    static int32 frame_counter = 0;

    //-------------------------------------------------------------------------
    void Application::on_app_update(const rex::FrameInfo& /*info*/)
    {
        ++frame_counter;

        if (frame_counter > 500)
        {
            quit();
        }
    }
}