#include "rex_scenegraph_pch.h"

#include "ecs/scene.h"
#include "ecs/entity.h"

#include "ecs/components/tag_component.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/model_component.h"
#include "ecs/components/material_component.h"
#include "ecs/components/camera_component.h"
#include "ecs/components/point_light_component.h"
#include "ecs/components/directional_light_component.h"

#include "model.h"

#include "renderer/renderer.h"
#include "renderer/point_light.h"
#include "renderer/directional_light.h"

#include "resources/material.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        rex::ref_ptr<Scene> Scene::create_empty()
        {
            return create_empty(ESID::SID_None);
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<Scene> Scene::create_empty(const StringID& name)
        {
            return create_empty(name, 0, 0);
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<Scene> Scene::create_empty(const StringID& name, int32 viewportWidth, int32 viewportHeight)
        {
            auto scene = make_ref<Scene>(name);

            scene->set_viewport_width(viewportWidth);
            scene->set_viewport_height(viewportHeight);

            return scene;
        }

        //-------------------------------------------------------------------------
        Scene::Scene(const StringID& name)
            : m_name(name)
            , m_viewport_width(0)
            , m_viewport_height(0)
        {
            m_environment.skybox_texture_cube = Renderer::get_black_texture_cube();
        }
        //-------------------------------------------------------------------------
        Scene::~Scene()
        {
        }

        //-------------------------------------------------------------------------
        void Scene::update()
        {
            auto point_lights = m_register.group<PointLightComponent>(entt::get<TransformComponent>);
            m_light_environment.point_lights.reserve(point_lights.size());
            for (auto entity : point_lights)
            {
                auto transform_comp = point_lights.get<TransformComponent>(entity);
                auto pointlight_comp = point_lights.get<PointLightComponent>(entity);

                PointLight light;
                light.intensity = pointlight_comp.intensity;
                light.min_attenuation = pointlight_comp.min_attenuation;
                light.max_attenuation = pointlight_comp.max_attenuation;
                light.position = transform_comp.transform.get_position();

                m_light_environment.point_lights.push_back(light);
            }

            auto dir_lights = m_register.group<DirectionalLightComponent>(entt::get<TransformComponent>);
            m_light_environment.directional_lights.reserve(dir_lights.size());
            for (auto entity : dir_lights)
            {
                auto transform_comp = dir_lights.get<TransformComponent>(entity);
                auto dirlight_comp = dir_lights.get<DirectionalLightComponent>(entity);

                if (dirlight_comp.is_active == false)
                {
                    continue;
                }

                DirectionalLight light;
                light.intensity = dirlight_comp.intensity;
                light.direction = -rex::normalize(rex::matrix3(transform_comp.transform.get_world()) * rex::vec3(1.0f));

                m_light_environment.directional_lights.push_back(light);
            }
        }

        //-------------------------------------------------------------------------
        rex::ecs::Entity Scene::create_entity()
        {
            return create_entity(ESID::SID_None);
        }
        //-------------------------------------------------------------------------
        rex::ecs::Entity Scene::create_entity(const StringID& name)
        {
            Entity entity = {m_register.create(), this};

            StringID entity_name = name == ESID::SID_None ? "entity"_sid : name;

            entity.add_component<TagComponent>(entity_name);
            entity.add_component<TransformComponent>();

            return entity;
        }

        //-------------------------------------------------------------------------
        void Scene::mark_for_post_poned_destroy_entity(Entity entity)
        {
            m_post_update_queue.emplace_back([&, entity]()
                                             {
                                                 mark_for_immediate_destroy_entity(entity);
                                             });
        }
        //-------------------------------------------------------------------------
        void Scene::mark_for_immediate_destroy_entity(Entity entity)
        {
            m_register.destroy(entity.get_handle());
        }

        //-------------------------------------------------------------------------
        void Scene::set_name(const StringID& name)
        {
            m_name = name;
        }
        //-------------------------------------------------------------------------
        void Scene::set_environment(const Environment& env)
        {
            m_environment = env;

            if (!m_environment)
            {
                m_environment.skybox_texture_cube = Renderer::get_black_texture_cube();
            }
        }

        //-------------------------------------------------------------------------
        void Scene::set_viewport_width(int32 width)
        {
            m_viewport_width = width;
        }

        //-------------------------------------------------------------------------
        void Scene::set_viewport_height(int32 height)
        {
            m_viewport_height = height;
        }

        //-------------------------------------------------------------------------
        const rex::StringID& Scene::get_name() const
        {
            return m_name;
        }
        //-------------------------------------------------------------------------
        const rex::Environment& Scene::get_environment() const
        {
            return m_environment;
        }

        //-------------------------------------------------------------------------
        const int32 Scene::get_viewport_width() const
        {
            return m_viewport_width;
        }

        //-------------------------------------------------------------------------
        const int32 Scene::get_viewport_height() const
        {
            return m_viewport_height;
        }

        //-------------------------------------------------------------------------
        const rex::LightEnvironment& Scene::get_light_environment() const
        {
            return m_light_environment;
        }

        //-------------------------------------------------------------------------
        entt::registry& Scene::get_registry()
        {
            return m_register;
        }
        //-------------------------------------------------------------------------
        const entt::registry& Scene::get_registry() const
        {
            return m_register;
        }

        //-------------------------------------------------------------------------
        size_t Scene::get_entity_count() const
        {
            return m_register.size();
        }
    }
}