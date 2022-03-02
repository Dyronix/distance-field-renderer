# ------------------------------- 
message(CHECK_START "Assemble project directories")

SET(FOUNDATIONS_LIB_PATH modules/foundations)

# --------------------------------
# opengl path
# --------------------------------
find_package(OpenGL REQUIRED)

set(OPENGL_INCLUDE_DIRECTORY ${OPENGL_INCLUDE_DIRS})
set(OPENGL_LIBRARY_NAME ${OPENGL_LIBRARIES})

# ------------------------------- 
# Library names
# -------------------------------
set(CASSIS_CORE core)
set(CASSIS_GLAD glad)

# --------------------------------
# core path
# --------------------------------
set(CURRENT_CORE_PATH ${CMAKE_CURRENT_LIST_DIR}/${FOUNDATIONS_LIB_PATH}/${CASSIS_CORE})

IF(NOT EXISTS ${CURRENT_CORE_PATH} OR NOT IS_DIRECTORY "${CURRENT_CORE_PATH}")
    message(FATAL_ERROR "Directory, ${CURRENT_CORE_PATH} does not exist, configuration will be stopped.")
ENDIF()

set(CORE_INCLUDE_DIRECTORY ${CURRENT_CORE_PATH}/include)
set(CORE_SOURCE_DIRECTORY ${CURRENT_CORE_PATH}/src)

set(CORE_LIBRARY_NAME c_${CASSIS_CORE})

# --------------------------------
# glad path
# --------------------------------
set(CURRENT_GLAD_PATH ${CMAKE_CURRENT_LIST_DIR}/${FOUNDATIONS_LIB_PATH}/${CASSIS_GLAD})

IF(NOT EXISTS ${CURRENT_GLAD_PATH} OR NOT IS_DIRECTORY "${CURRENT_GLAD_PATH}")
    message(FATAL_ERROR "Directory, ${CURRENT_GLAD_PATH} does not exist, configuration will be stopped.")
ENDIF()

set(GLAD_INCLUDE_DIRECTORY ${CURRENT_GLAD_PATH}/include)
set(GLAD_SOURCE_DIRECTORY ${CURRENT_GLAD_PATH}/src)

set(GLAD_LIBRARY_NAME c_${CASSIS_GLAD})

# ------------------------------- 
message(CHECK_PASS "Done") # Assemble project directories

# ------------------------------- 
# Setup externals
# -------------------------------
# Setup externals here ...

# ------------------------------- 
# Setup modules
# -------------------------------
message(STATUS "\tadd subdirectory: ${MODULES_DIR}")
add_subdirectory("${MODULES_DIR}")