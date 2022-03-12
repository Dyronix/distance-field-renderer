#pragma once

#include "data_type.h"

namespace rex
{
    namespace datatype_helper
    {
        //-------------------------------------------------------------------------
        inline std::unordered_map<size_t, DataType>& get_type_i_ds()
        {
            static std::unordered_map<size_t, DataType> type_ids = {{typeid(int8).hash_code(), rex::DataType::INT8},
                                                                    {typeid(int16).hash_code(), rex::DataType::INT16},
                                                                    {typeid(int32).hash_code(), rex::DataType::INT32},
                                                                    {typeid(int64).hash_code(), rex::DataType::INT64},
                                                                    {typeid(uint8).hash_code(), rex::DataType::UNSIGNED_INT8},
                                                                    {typeid(uint16).hash_code(), rex::DataType::UNSIGNED_INT16},
                                                                    {typeid(uint32).hash_code(), rex::DataType::UNSIGNED_INT32},
                                                                    {typeid(uint64).hash_code(), rex::DataType::UNSIGNED_INT64},
                                                                    {typeid(float).hash_code(), rex::DataType::FLOAT32},

                                                                    {typeid(rex::matrix3).hash_code(), rex::DataType::MAT3},
                                                                    {typeid(rex::matrix4).hash_code(), rex::DataType::MAT4},

                                                                    {typeid(rex::vec2).hash_code(), rex::DataType::VEC2},
                                                                    {typeid(rex::vec3).hash_code(), rex::DataType::VEC3},
                                                                    {typeid(rex::vec4).hash_code(), rex::DataType::VEC4},

                                                                    {typeid(rex::vec2i).hash_code(), rex::DataType::IVEC2},
                                                                    {typeid(rex::vec3i).hash_code(), rex::DataType::IVEC3},
                                                                    {typeid(rex::vec4i).hash_code(), rex::DataType::IVEC4},

                                                                    {typeid(rex::vec2u).hash_code(), rex::DataType::UIVEC2},
                                                                    {typeid(rex::vec3u).hash_code(), rex::DataType::UIVEC3},
                                                                    {typeid(rex::vec4u).hash_code(), rex::DataType::UIVEC4},

                                                                    {typeid(bool).hash_code(), rex::DataType::BOOL},
                                                                    {typeid(std::string).hash_code(), rex::DataType::STRING}};

            return type_ids;
        }

        //-------------------------------------------------------------------------
        template <typename T>
        inline bool is_of_type(const DataType& type)
        {
            auto it = get_type_i_ds().find(typeid(T).hash_code());
            if (it == std::cend(get_type_i_ds()))
            {
                return false;
            }

            return (*it).second == type;
        }

        //-------------------------------------------------------------------------
        template <typename T>
        inline const DataType get_type()
        {
            auto it = get_type_i_ds().find(typeid(T).hash_code());
            if (it == std::cend(get_type_i_ds()))
            {
                return DataType::NONE;
            }

            return (*it).second;
        }
    }
}