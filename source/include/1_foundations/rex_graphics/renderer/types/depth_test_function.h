#pragma once

namespace rex
{
    enum class DepthTestFunction
    {
        NONE,
        ALWAYS,
        NEVER,
        LESS,
        LESS_OR_EQUAL,
        GREATER,
        GREATER_OR_EQUAL,
        EQUAL,
        NOT_EQUAL,
    };

    //-------------------------------------------------------------------------
    std::ostream& operator<<(std::ostream& os, const DepthTestFunction& e);
}