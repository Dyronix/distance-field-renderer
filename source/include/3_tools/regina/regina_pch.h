#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <optional>
#include <memory>

#include <gsl/narrow>
#include <gsl/pointers>

#include "defines.h"
#include "types.h"

#include "named_type.h"

#include "rex_debug.h"

#include "memory/ref_counted_object.h"
#include "memory/ref_ptr_utilities.h"
#include "memory/ref_ptr.h"

#include "string/string_id.h"
#include "string/string_ids.h"

#include <glm/glm.hpp>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#pragma warning(pop)

#include "common.h"
#include "geometric.h"

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#include "matrix.h"
#include "matrix_clip_space.h"
#include "matrix_transform.h"

#include "quaternion.h"
#include "quaternion_geometric.h"

#include "deg_angle.h"
#include "rad_angle.h"

#include "algorithms/clamp.h"
#include "algorithms/identical.h"