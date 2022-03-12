#pragma once

namespace rex
{
    namespace memory
    {
        class size;
    }

    class DataType
    {
    public:
        enum Type
        {
            NONE = 0,

            INT8,
            INT16,
            INT32,
            INT64,

            UNSIGNED_INT8,
            UNSIGNED_INT16,
            UNSIGNED_INT32,
            UNSIGNED_INT64,

            FLOAT32,

            MAT3,
            MAT4,

            VEC2,
            VEC3,
            VEC4,

            IVEC2,
            IVEC3,
            IVEC4,

            UIVEC2,
            UIVEC3,
            UIVEC4,

            BOOL,

            SAMPLER2D,
            SAMPLER3D,
            SAMPLERCUBE,

            STRING
        };

        DataType(Type value);

        operator Type() const;

        size_t to_size() const;
        std::string to_string() const;

    private:
        Type m_value;
    };

    bool is_valid_type(DataType type);
    bool is_sampler_type(DataType type);
    bool is_vector_type(DataType type);
    bool is_matrix_type(DataType type);
    bool is_primitive_type(DataType type);

    namespace conversions
    {
        const std::string to_display_string(const DataType& type);
        const memory::size to_byte_size(const DataType& type);
    }
}