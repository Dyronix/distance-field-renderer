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
        TagComponent::TagComponent(const StringID& sid)
            : tag(sid)
        {
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
    }
}