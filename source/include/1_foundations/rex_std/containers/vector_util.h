#pragma once

namespace rex
{
    //-------------------------------------------------------------------------
    template <typename T>
    inline std::vector<T> create_vector_with_capacity(size_t capacity)
    {
        std::vector<T> vec;

        vec.reserve(capacity);

        return vec;
    }

    //-------------------------------------------------------------------------
    template <typename T>
    inline std::vector<T> create_vector_with_size(size_t size)
    {
        std::vector<T> vec;

        vec.resize(size);

        return vec;
    }
}