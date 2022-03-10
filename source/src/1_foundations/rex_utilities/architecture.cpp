#include "rex_utilities_pch.h"

#include "architecture.h"

namespace rex
{
    namespace architecture
    {
        //-------------------------------------------------------------------------
        const Type get_architecture_type()
        {
            switch (sizeof(void*)) 
            {
            case 4: return Type::BIT_32;
            case 8: return Type::BIT_64;
            }

            assert(false && "Unspecified architecture");
            return Type::UNSPECIFIED;
        }

        //-------------------------------------------------------------------------
        const bool is_architecture_32_bit()
        {
            return get_architecture_type() == Type::BIT_32;
        }
        //-------------------------------------------------------------------------
        const bool is_architecture_64_bit()
        {
            return get_architecture_type() == Type::BIT_64;
        }
    }
}