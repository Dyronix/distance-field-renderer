#pragma once

namespace rex
{
    namespace architecture
    {
        enum class Type
        {
            UNSPECIFIED,
            BIT_32,
            BIT_64
        };

        const Type get_architecture_type();

        const bool is_architecture_32_bit();
        const bool is_architecture_64_bit();
    };

    namespace configuration
    {
        const bool is_32_architecture = architecture::is_architecture_32_bit();
        const bool is_64_architecture = architecture::is_architecture_64_bit();
    }
}