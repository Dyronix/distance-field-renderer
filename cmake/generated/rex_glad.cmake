

# This file was automatically generated by "create_project.py"
# Command: generate_project(type project_name solution_folder project_folder pch_header_name = project_folder pch_src_name = project_folder)
# Example: generate_project(static CoreLibs 1_common core_libs)


# include path: ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_glad
# src path: ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_glad


# Project Name Project
# -------------------------
file(GLOB_RECURSE rex_glad_LIBS_INC    ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_glad/*.h)
file(GLOB_RECURSE rex_glad_LIBS_SRC    ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_glad/*.cpp; ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_glad/*.c)


# Create the project filters
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_glad include)
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_glad src)


# Create the project
add_library(rex_glad STATIC ${rex_glad_LIBS_INC} ${rex_glad_LIBS_SRC})

STRING(TOUPPER rex_glad UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)

# Set the include directories
target_include_directories(rex_glad PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_glad)


# Set project properties
set_target_properties(rex_glad PROPERTIES FOLDER                                         1_foundations) 
set_target_properties(rex_glad PROPERTIES DEFINE_SYMBOL                                  "" )                   
IF(MSVC)
	set_target_properties(rex_glad PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY                ${BIN_DIR})        
	set_target_properties(rex_glad PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                     ${BIN_DIR})        
	set_target_properties(rex_glad PROPERTIES LIBRARY_OUTPUT_DIRECTORY                     ${BIN_DIR})        
	set_target_properties(rex_glad PROPERTIES RUNTIME_OUTPUT_DIRECTORY                     ${BIN_DIR})        


	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_EnableUnitySupport                 True)                    
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_IncludeInUnityFile                 True)
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_OrderInUnityFile                   100)
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_CombineFilesOnlyFromTheSameFolder  false)
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_MinFilesInUnityFile                2)
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_MaxFilesInUnityFile                0)
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_MinUnityFiles                      1)
	set_target_properties(rex_glad PROPERTIES VS_GLOBAL_UnityFilesDirectory                .)
ENDIF()


# Compiler options
IF(MSVC)
	target_compile_options(rex_glad PRIVATE /W4 /WX /MP)
ENDIF()


# # Set precompiled header
# add_precompiled_header(rex_glad "rex_glad_pch.h" "rex_glad_pch.cpp")