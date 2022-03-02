# --------------------------
# Setup sources
# --------------------------
message(CHECK_START "Generating ${GLAD_LIBRARY_NAME}")

file(GLOB_RECURSE TARGET_INC "${GLAD_INCLUDE_DIRECTORY}/*.h")
file(GLOB_RECURSE TARGET_SRC "${GLAD_SOURCE_DIRECTORY}/*.c")

GROUPSOURCES("include")
GROUPSOURCES("src")

# --------------------------
# Setup library
# --------------------------
add_library(${GLAD_LIBRARY_NAME} STATIC
            ${TARGET_INC}
            ${TARGET_RC}
            ${TARGET_SRC})

STRING(TOUPPER ${GLAD_LIBRARY_NAME} UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)

# --------------------------
# Target properties
# --------------------------
set_target_properties(${GLAD_LIBRARY_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_BINDIR}")
set_target_properties(${GLAD_LIBRARY_NAME} PROPERTIES FOLDER 1_foundations)
set_target_properties(${GLAD_LIBRARY_NAME} PROPERTIES DEFINE_SYMBOL "" )

# --------------------------
# Target include directories.
# --------------------------
target_include_directories(${GLAD_LIBRARY_NAME} PUBLIC     
                                                $<BUILD_INTERFACE:${GLAD_INCLUDE_DIRECTORY}>
                                                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${GLAD_LIBRARY_NAME}>)
                                                
message(CHECK_PASS "Done")