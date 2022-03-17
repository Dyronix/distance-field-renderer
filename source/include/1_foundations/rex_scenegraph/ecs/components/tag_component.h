#pragma once

namespace rex
{
    namespace ecs
    {
        struct TagComponent
        {
            TagComponent();
            TagComponent(const TagComponent& other);
            TagComponent(TagComponent&& other) noexcept;
            TagComponent(const StringID& sid);

            TagComponent& operator=(const TagComponent& other);
            TagComponent& operator=(TagComponent&& other) noexcept;

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