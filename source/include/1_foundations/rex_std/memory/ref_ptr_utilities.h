#pragma once

#include "memory/ref_counted_object.h"

#include <unordered_set>

namespace rex
{
    using RefCountedObjectsContainer = std::unordered_set<const rex::IRefCountedObject*>;

    namespace memory
    {
        void add_to_reference_pool(const rex::IRefCountedObject* instance);
        void remove_from_reference_pool(const rex::IRefCountedObject* instance);

        const RefCountedObjectsContainer& get_all_references();

        bool is_alive(const rex::IRefCountedObject* instance);
    }
}