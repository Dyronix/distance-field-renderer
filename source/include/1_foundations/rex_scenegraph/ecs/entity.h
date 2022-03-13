#pragma once

#include "transform.h"

#include <entt/entt.hpp>

namespace rex
{
    namespace ecs
    {
        class Scene;

        class Entity
        {
        public:
            Entity();
            Entity(entt::entity handle, Scene* scene);
            Entity(const Entity& other);
            ~Entity();

            rex::Transform&     transform();
            const Transform&    transform() const;

            bool                is_null() const;

            StringID&           get_name();
            const StringID&     get_name() const;

            const entt::entity  get_handle() const;

            template<typename T, typename... Args>
            T&                  add_component(Args&&... args);
            template<typename T>
            void                remove_component();

            template<typename T>
            T&                  get_component();
            template<typename T>
            const T&            get_component() const;

            template <typename... T>
            auto                get_components();
            template<typename... T>
            const auto          get_components() const;

            template<typename... T>
            bool                has_component();
            template<typename... T>
            bool                has_component() const;

            template<typename... T>
            bool                has_any();
            template<typename... T>
            bool                has_any() const;

            template<typename... T>
            bool                has_all();
            template<typename... T>
            bool                has_all() const;

            bool operator==(const Entity& other);
            bool operator!=(const Entity& other);

        private:
            entt::entity    m_entity_handle;
            Scene*          m_scene;
        };

        //-------------------------------------------------------------------------
        template<typename T, typename... Args>
        T& Entity::add_component(Args&&... args)
        {
            R_ASSERT_X(!has_any<T>(), "Entity already has component!");
            return m_scene->get_registry().emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        }
        //-------------------------------------------------------------------------
        template<typename T>
        void Entity::remove_component()
        {
            R_ASSERT_X(has_any<T>(), "Entity does not have component!");
            m_scene->get_registry().remove<T>(m_entity_handle);
        }

        //-------------------------------------------------------------------------
        template<typename T>
        T& Entity::get_component()
        {
            R_ASSERT_X(has_any<T>(), "Entity does not have component!");
            return m_scene->get_registry().get<T>(m_entity_handle);
        }
        //-------------------------------------------------------------------------
        template<typename T>
        const T& Entity::get_component() const
        {
            R_ASSERT_X(has_any<T>(), "Entity does not have component!");
            return m_scene->get_registry().get<T>(m_entity_handle);
        }

        //-------------------------------------------------------------------------
        template <typename... T>
        auto Entity::get_components()
        {
            return m_scene->get_registry().group_if_exists<T...>();
        }

        //-------------------------------------------------------------------------
        template<typename... T>
        const auto Entity::get_components() const
        {
            return m_scene->get_registry().group_if_exists<T...>();
        }

        //-------------------------------------------------------------------------
        template<typename... T>
        bool Entity::has_any()
        {
            return m_scene->get_registry().any_of<T...>(m_entity_handle);
        }
        //-------------------------------------------------------------------------
        template<typename... T>
        bool Entity::has_any() const
        {
            return m_scene->get_registry().any_of<T...>(m_entity_handle);
        }

        //-------------------------------------------------------------------------
        template<typename... T>
        bool Entity::has_all()
        {
            return m_scene->get_registry().all_of<T...>(m_entity_handle);
        }
        //-------------------------------------------------------------------------
        template<typename... T>
        bool Entity::has_all() const
        {
            return m_scene->get_registry().all_of<T...>(m_entity_handle);
        }
    }
}