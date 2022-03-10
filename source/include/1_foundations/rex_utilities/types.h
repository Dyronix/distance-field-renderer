//--------------------------------------------------
// File:    types.h
// Created: 12/11/2021 2:27:17 PM
//
// Created By: nickdb
// Modified By: davedb
//
// Copyright (c) REX
//--------------------------------------------------

#pragma once

#include "yesnoenum.h"

namespace rex
{
    // integers

    using int8 = signed char;
    using int16 = signed short;
    using int32 = signed int;
    using int64 = signed long long;

    static_assert(sizeof(int8) == 1, "int8 must be 1 byte big");
    static_assert(sizeof(int16) == 2, "int16 must be 2 bytes big");
    static_assert(sizeof(int32) == 4, "int32 must be 4 bytes big");
    static_assert(sizeof(int64) == 8, "int64 must be 8 bytes big");

    using uint8 = unsigned char;
    using uint16 = unsigned short;
    using uint32 = unsigned int;
    using uint64 = unsigned long long;

    static_assert(sizeof(uint8) == 1, "uint8 must be 1 byte big");
    static_assert(sizeof(uint16) == 2, "uint16 must be 2 bytes big");
    static_assert(sizeof(uint32) == 4, "uint32 must be 4 bytes big");
    static_assert(sizeof(uint64) == 8, "uint64 must be 8 bytes big");

    // Yes, cardinals are meant to always be positive,
    // so it makes sense for them to be unsigned.
    // however, because we don't want to use unsigned types
    // because they're such a pain,
    // cardinals are signed, but their value should always be positive
    using card8 = int8;
    using card16 = int16;
    using card32 = int32;
    using card64 = int64;

    // characters
    using char8 = char;
    using char16 = wchar_t;

    static_assert(sizeof(char8) == 1, "char8 must be 1 byte big");
    static_assert(sizeof(char16) == 2, "char16 must be 2 bytes big");

    // floating-point
    using float32 = float;
    using float64 = double;

    static_assert(sizeof(float32) == 4, "float32 must be 4 bytes big");
    static_assert(sizeof(float64) == 8, "float64 must be 8 bytes big");

    namespace internal
    {
        // unscoped enums have an implementation defined underlying type not larger than int
        // this means it can be smaller, which could cause some hoorrible bugs when reading/writing data
        // on different platforms
        enum test_enum
        {
        };
        static_assert(sizeof(test_enum) == sizeof(int32), "unscoped enum must be 4 bytes big");

        // scoped enums default to underlying type is int, so let's make sure it actually is
        enum class test_scoped_enum
        {
        };
        static_assert(sizeof(test_scoped_enum) == sizeof(int32), "scoped enum must be 4 bytes big");
    }

    static_assert(false == 0, "C++ false keyword does not equal 0");
    static_assert(true == 1, "C++ true keyword does not equal 1");

    // specific types
    using IsRenderThread = YesNoEnum;

    namespace memory
    {
        enum class byte : unsigned char
        {
        };
    }
}

using rex::int8;
using rex::int16;
using rex::int32;
using rex::int64;

using rex::uint8;
using rex::uint16;
using rex::uint32;
using rex::uint64;

using rex::card8;
using rex::card16;
using rex::card32;
using rex::card64;

using rex::char8;
using rex::char16;

using rex::float32;
using rex::float64;