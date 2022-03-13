#include "rex_scenegraph_pch.h"

#include "ecs/entity.h"

#include "ecs/scene.h"
#include "ecs/components/transform_component.h"
#include "ecs/components/tag_component.h"

namespace rex
{
    namespace ecs
    {

        //-------------------------------------------------------------------------
        Entity::Entity()
            :m_entity_handle(entt::null)
            ,m_scene(nullptr)
        {
        }

        //-------------------------------------------------------------------------
        Entity::Entity(entt::entity handle, Scene* scene)
            :m_entity_handle(handle)
            ,m_scene(scene)
        {

        }

        //-------------------------------------------------------------------------
        Entity::Entity(const Entity& other)
            :m_entity_handle(other.m_entity_handle)
            ,m_scene(other.m_scene)
        {

        }

        //-------------------------------------------------------------------------
        Entity::~Entity()
        {

        }

        //-------------------------------------------------------------------------
        rex::Transform& Entity::transform()
        {
            TransformComponent& transform_comp = m_scene->get_registry().get<TransformComponent>(m_entity_handle);
            return transform_comp;
        }
        //-------------------------------------------------------------------------
        const Transform& Entity::transform() const
        {
            const TransformComponent& transform_comp = m_scene->get_registry().get<TransformComponent>(m_entity_handle);
            return transform_comp;
        }

        //-------------------------------------------------------------------------
        bool Entity::is_null() const
        {
            return m_entity_handle == entt::null;
        }

        //-------------------------------------------------------------------------
        StringID& Entity::get_name()
        {
            TagComponent& tag_comp = m_scene->get_registry().get<TagComponent>(m_entity_handle);
            return tag_comp;
        }
        //-------------------------------------------------------------------------
        const StringID& Entity::get_name() const
        {
            StringID& tag_comp = m_scene->get_registry().get<TagComponent>(m_entity_handle);
            return tag_comp;
        }

        //-------------------------------------------------------------------------
        const entt::entity Entity::get_handle() const
        {
            return m_entity_handle;
        }

        //-------------------------------------------------------------------------
        bool Entity::operator==(const Entity& other)
        {
            return m_entity_handle == other.m_entity_handle;
        }

        //-------------------------------------------------------------------------
        bool Entity::operator!=(const Entity& other)
        {
            return !(*this == other);
        }
    }
}