#include "rex_memory_pch.h"

#include "data_type.h"
#include "memory_size.h"

namespace rex
{
    //-------------------------------------------------------------------------
    DataType::DataType(Type value)
        : m_value(value)
    {
    }

    //-------------------------------------------------------------------------
    DataType::operator Type() const
    {
        return m_value;
    }

    //-------------------------------------------------------------------------
    size_t DataType::to_size() const
    {
        switch (m_value)
        {
            case DataType::NONE: return 0;

            case DataType::INT8: return sizeof(int8);
            case DataType::INT16: return sizeof(int16);
            case DataType::INT32: return sizeof(int32);
            case DataType::INT64: return sizeof(int64);

            case DataType::UNSIGNED_INT8: return sizeof(uint8);
            case DataType::UNSIGNED_INT16: return sizeof(uint16);
            case DataType::UNSIGNED_INT32: return sizeof(uint32);
            case DataType::UNSIGNED_INT64: return sizeof(uint64);

            case DataType::FLOAT32: return sizeof(float);

            case DataType::MAT3: return sizeof(float) * 3 * 3;
            case DataType::MAT4: return sizeof(float) * 4 * 4;

            case DataType::VEC2: return sizeof(float) * 2;
            case DataType::VEC3: return sizeof(float) * 3;
            case DataType::VEC4: return sizeof(float) * 4;

            case DataType::IVEC2: return sizeof(int32) * 2;
            case DataType::IVEC3: return sizeof(int32) * 3;
            case DataType::IVEC4: return sizeof(int32) * 4;

            case DataType::UIVEC2: return sizeof(uint32) * 2;
            case DataType::UIVEC3: return sizeof(uint32) * 3;
            case DataType::UIVEC4: return sizeof(uint32) * 4;

            case DataType::SAMPLER2D: return sizeof(int32);
            case DataType::SAMPLER3D: return sizeof(int32);
            case DataType::SAMPLERCUBE: return sizeof(int32);

            case DataType::BOOL: return sizeof(bool);
            case DataType::STRING: std::cout << ("Size of string cannot be defined use: std::string::size() * sizeof(std::string::operator[](int))"); return gsl::narrow<size_t>(-1);
        }

        assert(false && "Unknown DataType!");
        return 0;
    }
    //-------------------------------------------------------------------------
    std::string DataType::to_string() const
    {
        switch (m_value)
        {
            case DataType::NONE: return "None";

            case DataType::INT8: return "Int8";
            case DataType::INT16: return "Int16";
            case DataType::INT32: return "Int32";
            case DataType::INT64: return "Int64";

            case DataType::UNSIGNED_INT8: return "Unsigned Int8";
            case DataType::UNSIGNED_INT16: return "Unsigned Int16";
            case DataType::UNSIGNED_INT32: return "Unsigned Int32";
            case DataType::UNSIGNED_INT64: return "Unsigned Int64";

            case DataType::FLOAT32: return "Float32";

            case DataType::MAT3: return "Mat3";
            case DataType::MAT4: return "Mat4";

            case DataType::VEC2: return "Vec2";
            case DataType::VEC3: return "Vec3";
            case DataType::VEC4: return "Vec4";

            case DataType::IVEC2: return "IVec2";
            case DataType::IVEC3: return "IVec3";
            case DataType::IVEC4: return "IVec4";

            case DataType::UIVEC2: return "UIVec2";
            case DataType::UIVEC3: return "UIVec3";
            case DataType::UIVEC4: return "UIVec4";

            case DataType::SAMPLER2D: return "Sampler 2D";
            case DataType::SAMPLER3D: return "Sampler 3D";
            case DataType::SAMPLERCUBE: return "Sampler Cube";

            case DataType::BOOL: return "Bool";
            case DataType::STRING: return "String";
        }

        assert(false && "Unknown DataType!");
        return 0;
    }

    //-------------------------------------------------------------------------
    bool is_valid_type(DataType type)
    {
        return type != DataType::NONE;
    }

    //-------------------------------------------------------------------------
    bool is_sampler_type(DataType type)
    {
        return type == DataType::SAMPLER2D || type == DataType::SAMPLER3D || type == DataType::SAMPLERCUBE;
    }

    //-------------------------------------------------------------------------
    bool is_vector_type(DataType type)
    {
        return type == DataType::IVEC2 || type == DataType::IVEC3 || type == DataType::IVEC4 || type == DataType::UIVEC2 || type == DataType::UIVEC3 || type == DataType::UIVEC4 || type == DataType::VEC2 || type == DataType::VEC3 ||
               type == DataType::VEC4;
    }

    //-------------------------------------------------------------------------
    bool is_matrix_type(DataType type)
    {
        return type == DataType::MAT3 || type == DataType::MAT4;
    }

    //-------------------------------------------------------------------------
    bool is_primitive_type(DataType type)
    {
        return type == DataType::BOOL || type == DataType::FLOAT32 || type == DataType::INT8 || type == DataType::INT16 || type == DataType::INT32 || type == DataType::INT64 || type == DataType::UNSIGNED_INT8 || type == DataType::UNSIGNED_INT16 ||
               type == DataType::UNSIGNED_INT32 || type == DataType::UNSIGNED_INT64;
    }

    namespace conversions
    {
        //-------------------------------------------------------------------------
        const std::string to_display_string(const DataType& type)
        {
            switch (type)
            {
                case DataType::NONE: return "None";

                case DataType::BOOL: return "Boolean";
                case DataType::INT8: return "8 Bit Integer";
                case DataType::INT16: return "16 Bit Integer";
                case DataType::INT32: return "32 Bit Integer";
                case DataType::INT64: return "64 Bit Integer";
                case DataType::UNSIGNED_INT8: return "8 Bit Unsigned Integer";
                case DataType::UNSIGNED_INT16: return "16 Bit Unsigned Integer";
                case DataType::UNSIGNED_INT32: return "32 Bit Unsigned Integer";
                case DataType::UNSIGNED_INT64: return "64 Bit Unsigned Integer";
                case DataType::FLOAT32: return "32 Bit Floating Point";
                case DataType::VEC2: return "Vector 2D";
                case DataType::VEC3: return "Vector 3D";
                case DataType::VEC4: return "Vector 4D";
                case DataType::MAT3: return "Matrix 3x3";
                case DataType::MAT4: return "Matrix 4x4";
                case DataType::IVEC2: return "Integer Vector 2D";
                case DataType::IVEC3: return "Integer Vector 3D";
                case DataType::IVEC4: return "Integer Vector 4D";
                case DataType::UIVEC2: return "Unsigned Integer Vector 2D";
                case DataType::UIVEC3: return "Unsigned Integer Vector 3D";
                case DataType::UIVEC4: return "Unsigned Integer Vector 4D";
                case DataType::SAMPLER2D: return "Sampler 2D";
                case DataType::SAMPLER3D: return "Sampler 3D";
                case DataType::SAMPLERCUBE: return "Sampler Cube";
                case DataType::STRING: return "String";
            }

            R_ASSERT_X(false, "Invalid Data Type");
            return "";
        }

        //-------------------------------------------------------------------------
        const rex::memory::size to_byte_size(const DataType& type)
        {
            switch (type)
            {
                case DataType::NONE: return 0_bytes;

                case DataType::BOOL: return sizeof(uint32);
                case DataType::INT8: return sizeof(int8);
                case DataType::INT16: return sizeof(int16);
                case DataType::INT32: return sizeof(int32);
                case DataType::INT64: return sizeof(int64);
                case DataType::UNSIGNED_INT8: return sizeof(uint8);
                case DataType::UNSIGNED_INT16: return sizeof(uint16);
                case DataType::UNSIGNED_INT32: return sizeof(uint32);
                case DataType::UNSIGNED_INT64: return sizeof(uint64);
                case DataType::FLOAT32: static_assert(sizeof(float) == 4); return sizeof(float);
                case DataType::VEC2: return sizeof(float) * 2;
                case DataType::VEC3: return sizeof(float) * 3;
                case DataType::VEC4: return sizeof(float) * 4;
                case DataType::MAT3: return sizeof(float) * 3 * 3;
                case DataType::MAT4: return sizeof(float) * 4 * 4;
                case DataType::IVEC2: return sizeof(int32) * 2;
                case DataType::IVEC3: return sizeof(int32) * 3;
                case DataType::IVEC4: return sizeof(int32) * 4;
                case DataType::UIVEC2: return sizeof(uint32) * 2;
                case DataType::UIVEC3: return sizeof(uint32) * 3;
                case DataType::UIVEC4: return sizeof(uint32) * 4;

                case DataType::SAMPLER2D: return sizeof(uint32);
                case DataType::SAMPLER3D: return sizeof(uint32);
                case DataType::SAMPLERCUBE: return sizeof(uint32);

                case DataType::STRING: std::cout << ("Size of string cannot be defined use: std::string::size() * sizeof(std::string::operator[](int))"); return gsl::narrow<size_t>(-1);
            }

            R_ASSERT_X(false, "Invalid Data Type");
            return 0_bytes;
        }
    }
}