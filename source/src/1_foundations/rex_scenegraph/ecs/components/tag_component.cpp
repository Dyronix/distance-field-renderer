#include "rex_scenegraph_pch.h"

#include "ecs/components/tag_component.h"

namespace rex
{
    namespace ecs
    {
        //-------------------------------------------------------------------------
        TagComponent::TagComponent()
            : tag(ESID::SID_None)
        {
        }
        //-------------------------------------------------------------------------
        TagComponent::TagComponent(const TagComponent& other)
            : tag(other.tag)
        {
        }
        //-------------------------------------------------------------------------
        TagComponent::TagComponent(TagComponent&& other) noexcept
            : tag(std::exchange(other.tag, {}))
        {
        }
        //-------------------------------------------------------------------------
        TagComponent::TagComponent(const StringID& sid)
            : tag(sid)
        {
        }

        //-------------------------------------------------------------------------
        rex::ecs::TagComponent& TagComponent::operator=(const TagComponent& other)
        {
            this->tag = other.tag;
            return *this;
        }

        //-------------------------------------------------------------------------
        rex::ecs::TagComponent& TagComponent::operator=(TagComponent&& other) noexcept
        {
            this->tag = std::exchange(other.tag, {});
            return *this;
        }

        //-------------------------------------------------------------------------
        TagComponent::operator StringID&()
        {
            return tag;
        }
        //-------------------------------------------------------------------------
        TagComponent::operator const StringID&() const
        {
            return tag;
        }

        //-------------------------------------------------------------------------
        TagComponent::operator std::string()
        {
            return tag.to_string();
        }
        //-------------------------------------------------------------------------
        TagComponent::operator const std::string() const
        {
            return tag.to_string();
        }

        //-------------------------------------------------------------------------
        bool TagComponent::operator==(const TagComponent& other) const
        {
            return tag == other.tag;
        }
        //-------------------------------------------------------------------------
        bool TagComponent::operator!=(const TagComponent& other) const
        {
            return !(*this == other);
        }
    } // namespace ecs
} // namespace rex