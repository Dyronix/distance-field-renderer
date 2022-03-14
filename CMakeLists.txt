# -------------------------------
# Setup CMake minimum version
# -------------------------------
cmake_minimum_required(VERSION 3.16)

# ------------------------------- 
# Options
# ------------------------------- 
option(REX_UNITY_BUILD "Use unity build" OFF)
option(REX_EMSCRIPTEN_BUILD "Use emscripten build" OFF)

# ------------------------------- 
# Build Targets     
# ------------------------------- 
set(WINDOWS WINDOWS)
set(MAC MACOS)
set(LINUX LINUX)

set(PLATFORM_TARGET "")

# -------------------------------
# Find Build Target
# -------------------------------
IF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(PLATFORM_TARGET ${WINDOWS})
ELSEIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM_TARGET ${LINUX})
ELSEIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(PLATFORM_TARGET ${MAC})
ELSE()
    message(FATAL_ERROR "Unsupported platform, configuration will end.")
ENDIF()

message(STATUS "Build Target: ${PLATFORM_TARGET}")

string(TOLOWER ${PLATFORM_TARGET} LOWER_PLATFORM_TARGET)
string(TOUPPER ${PLATFORM_TARGET} UPPER_PLATFORM_TARGET)

# ------------------------------- 
# API Targets     
# ------------------------------- 
set(OPENGL OPENGL)
set(OPENGLES OPENGLES)

set(GRAPHICS_API_TARGET "")

# -------------------------------
# Find API Target
# -------------------------------
IF(NOT REX_EMSCRIPTEN_BUILD)
    IF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
		set(GRAPHICS_API_TARGET ${OPENGL})
    ELSEIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
        set(GRAPHICS_API_TARGET ${OPENGL})
    ELSEIF(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
        set(GRAPHICS_API_TARGET ${OPENGL})
    ELSE()
        message(FATAL_ERROR "Unsupported platform, configuration will end.")
    ENDIF()
ELSE()
    set(GRAPHICS_API_TARGET ${OPENGLES})
ENDIF()

message(STATUS "Graphics API: ${GRAPHICS_API_TARGET}")

string(TOLOWER ${GRAPHICS_API_TARGET} LOWER_GRAPHICS_API_TARGET)
string(TOUPPER ${GRAPHICS_API_TARGET} UPPER_GRAPHICS_API_TARGET)

# -------------------------------
# Version
# -------------------------------
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 0)

# -------------------------------
# Configurations
# -------------------------------
SET(CONFIGURATION_DEBUG Debug)
SET(CONFIGURATION_RELWITHDEBINFO RelWithDebInfo)
SET(CONFIGURATION_MINSIZEREL Release)
SET(CONFIGURATION_RELEASE Release)

# -------------------------------
# Setup the solution
# -------------------------------
project(rex_${LOWER_PLATFORM_TARGET}_${LOWER_GRAPHICS_API_TARGET} VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" LANGUAGES C;CXX)

message(STATUS "#--------------------- Starting project generation ---------------------")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")
message(STATUS "Generating project: ${PROJECT_NAME} v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# -------------------------------
# CMake dependencies
# -------------------------------
include(cmake\\utilities.cmake)
include(cmake\\directories.cmake)
include(cmake\\precompile_header.cmake)

# -------------------------------
# Project directories
# -------------------------------
message(STATUS "Bin directory: ${BIN_DIR}")
message(STATUS "Source directory: ${SRC_DIR}")

# -------------------------------
# Setup project output directories
# -------------------------------
# Debug
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 			"${BIN_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 			"${BIN_DIR}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 			"${BIN_DIR}")
# Release configuration with minimum size
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL 		"${BIN_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL 		"${BIN_DIR}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL 		"${BIN_DIR}")
# Release configuration with debug information
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO 	"${BIN_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO 	"${BIN_DIR}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO 	"${BIN_DIR}")
# Release configuration
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 		"${BIN_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE 		"${BIN_DIR}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE 		"${BIN_DIR}")

# -------------------------------
# Setup debugging flags
# -------------------------------
SET(CMAKE_C_FLAGS_DEBUG                     "${CMAKE_C_FLAGS_DEBUG}                     -DREX_DEBUG -DREX_ASSERTIONS")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO            "${CMAKE_C_FLAGS_RELWITHDEBINFO}            -DREX_DEBUG -DREX_ASSERTIONS")
SET(CMAKE_C_FLAGS_MINSIZEREL                "${CMAKE_C_FLAGS_MINSIZEREL}                ")
SET(CMAKE_C_FLAGS_RELEASE                   "${CMAKE_C_FLAGS_RELEASE}                   ")
    
SET(CMAKE_CXX_FLAGS_DEBUG                   "${CMAKE_CXX_FLAGS_DEBUG}                   -DREX_DEBUG -DREX_ASSERTIONS")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO          "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}          -DREX_DEBUG -DREX_ASSERTIONS")
SET(CMAKE_CXX_FLAGS_MINSIZEREL              "${CMAKE_CXX_FLAGS_MINSIZEREL}              ")
SET(CMAKE_CXX_FLAGS_RELEASE                 "${CMAKE_CXX_FLAGS_RELEASE}                 ")

# --------------------------
# Setup global properties
# --------------------------
# Enable folder structure in the IDE
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
	# Remove the always defined _WINDOWS flag
	# CPP
	string (REPLACE "/D_WINDOWS" "" CMAKE_CUSTOM_CPP_FLAGS ${CMAKE_CXX_FLAGS})
	string (REPLACE "/DWIN32" "" CMAKE_CUSTOM_CPP_FLAGS ${CMAKE_CUSTOM_CPP_FLAGS}) 
	SET(CMAKE_CXX_FLAGS "${CMAKE_CUSTOM_CPP_FLAGS}" CACHE STRING "" FORCE)
	# C
	string (REPLACE "/D_WINDOWS" "" CMAKE_CUSTOM_C_FLAGS ${CMAKE_C_FLAGS})
	string (REPLACE "/DWIN32" "" CMAKE_CUSTOM_C_FLAGS ${CMAKE_CUSTOM_C_FLAGS})
	SET(CMAKE_C_FLAGS "${CMAKE_CUSTOM_C_FLAGS}" CACHE STRING "" FORCE)
endif()

# Debug postfix
SET (CMAKE_DEBUG_POSTFIX _d)

# --------------------------------
# Setup compiler features
# --------------------------------
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")

# --------------------------------
# Setup compiler definitions
# --------------------------------
add_definitions(-DREX_PLATFORM_${UPPER_PLATFORM_TARGET})
add_definitions(-DREX_API_${UPPER_GRAPHICS_API_TARGET})


# --------------------------------
# Add subdirectories
# --------------------------------
add_subdirectory(${EXTERNALS_DIR}/assimp-${LOWER_PLATFORM_TARGET})
add_subdirectory(${SRC_DIR})

message(STATUS "#---------------------- Ending project generation ----------------------")