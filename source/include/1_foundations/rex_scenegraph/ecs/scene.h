#pragma once

#include "renderer/environment.h"

#include <entt/entt.hpp>

namespace rex
{
    struct FrameInfo;

    namespace ecs
    {
        class Entity;

        class Scene : public RefCountedObject
        {
        public:
            static ref_ptr<Scene> create_empty();
            static ref_ptr<Scene> create_empty(const StringID& name);
            static ref_ptr<Scene> create_empty(const StringID& name, int32 viewportWidth, int32 viewportHeight);

            Scene(const StringID& name);
            ~Scene();

            void                    update();

            Entity                  create_entity();
            Entity                  create_entity(const StringID& name);

            void                    mark_for_post_poned_destroy_entity(Entity entity);
            void                    mark_for_immediate_destroy_entity(Entity entity);

            void                    set_name(const StringID& name);
            void                    set_environment(const Environment& env);
            void                    set_viewport_width(int32 width);
            void                    set_viewport_height(int32 height);

            const StringID&         get_name() const;
            const Environment&      get_environment() const;
            const int32             get_viewport_width() const;
            const int32             get_viewport_height() const;
            const LightEnvironment& get_light_environment() const;

            entt::registry&         get_registry();
            const entt::registry&   get_registry() const;

            size_t                  get_entity_count() const;

        private:
            using PostUpdateQueue = std::vector<std::function<void()>>;

            StringID                m_name;

            Environment             m_environment;
            LightEnvironment        m_light_environment;

            entt::registry          m_register;

            PostUpdateQueue         m_post_update_queue;

            int32                   m_viewport_width;
            int32                   m_viewport_height;
        };
    }
}