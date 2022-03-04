#pragma once

#include <exception>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>

#if __cplusplus >= 201703L // C++17 and later 
    #include <string_view>
#endif

#include <gsl/narrow>

#include "types.h"