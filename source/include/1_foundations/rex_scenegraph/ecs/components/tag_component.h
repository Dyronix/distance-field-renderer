#pragma once

namespace rex
{
    namespace ecs
    {
        struct TagComponent
        {
            TagComponent();
            TagComponent(const TagComponent& other);
            TagComponent(const StringID& sid);

            operator StringID&();
            operator const StringID&() const;

            operator std::string();
            operator const std::string() const;

            bool operator==(const TagComponent& other) const;
            bool operator!=(const TagComponent& other) const;

            StringID tag;
        };
    }
}