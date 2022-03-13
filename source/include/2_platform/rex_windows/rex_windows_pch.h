#pragma once

#include <optional>
#include <algorithm>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <unordered_map>
#include <sstream>
#include <ostream>

#include <gsl/narrow>
#include <gsl/pointers>

#include "types.h"
#include "defines.h"

#include "named_type.h"

#include "rex_debug.h"

#include "memory/ref_counted_object.h"
#include "memory/ref_ptr.h"
#include "memory/ref_ptr_utilities.h"

#include "string/string_id.h"
#include "string/string_ids.h"

#include "graphics/gl_api.h"
#include "graphics/gl_error.h"
#include "graphics/gl_function_library.h"

#include "common.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "matrix.h"