# --------------------------
# Setup sources
# --------------------------
message(CHECK_START "Generating ${CORE_LIBRARY_NAME}")

file(GLOB_RECURSE TARGET_INC "${CORE_INCLUDE_DIRECTORY}/*.h")
file(GLOB_RECURSE TARGET_SRC "${CORE_SOURCE_DIRECTORY}/*.cpp")

GROUPSOURCES("include")
GROUPSOURCES("src")

# --------------------------
# Setup library
# --------------------------
add_library(${CORE_LIBRARY_NAME} STATIC
            ${TARGET_INC}
            ${TARGET_RC}
            ${TARGET_SRC})

STRING(TOUPPER ${CORE_LIBRARY_NAME} UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)

# --------------------------
# Target properties
# --------------------------
set_target_properties(${CORE_LIBRARY_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_BINDIR}")
set_target_properties(${CORE_LIBRARY_NAME} PROPERTIES FOLDER 1_foundations)
set_target_properties(${CORE_LIBRARY_NAME} PROPERTIES DEFINE_SYMBOL "" )

# --------------------------
# Target include directories.
# --------------------------
target_include_directories(${CORE_LIBRARY_NAME} PUBLIC     
                                                $<BUILD_INTERFACE:${CORE_INCLUDE_DIRECTORY}>
                                                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CORE_LIBRARY_NAME}>)

target_include_directories(${CORE_LIBRARY_NAME} PUBLIC ${GLAD_INCLUDE_DIRECTORY})

# --------------------------
# Target link libraries.
# --------------------------
target_link_libraries(${CORE_LIBRARY_NAME} PUBLIC ${GLAD_LIBRARY_NAME})


# --------------------------
# Precompiled Header
# --------------------------
add_precompiled_header(${CORE_LIBRARY_NAME} "${CORE_INCLUDE_DIRECTORY}/${CORE_LIBRARY_NAME}_pch.h" "${CORE_SOURCE_DIRECTORY}/${CORE_LIBRARY_NAME}_pch.cpp")

message(CHECK_PASS "Done")