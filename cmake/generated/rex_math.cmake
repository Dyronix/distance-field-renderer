

# This file was automatically generated by "create_project.py"
# Command: generate_project(type project_name solution_folder project_folder pch_header_name = project_folder pch_src_name = project_folder)
# Example: generate_project(static CoreLibs 1_common core_libs)


# include path: ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_math
# src path: ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_math


# Project Name Project
# -------------------------
file(GLOB_RECURSE rex_math_LIBS_INC    ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_math/*.h)
file(GLOB_RECURSE rex_math_LIBS_SRC    ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_math/*.cpp; ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_math/*.c)


# Create the project filters
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_math include)
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_math src)


# Create the project
add_library(rex_math STATIC ${rex_math_LIBS_INC} ${rex_math_LIBS_SRC})

STRING(TOUPPER rex_math UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)


# Set the include directories                                                                                                  
target_include_directories(rex_math PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_math)                
target_include_directories(rex_math PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_diagnostics)                
target_include_directories(rex_math PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_utilities)                
target_include_directories(rex_math PUBLIC ${INCLUDE_GLM})          

target_link_libraries(rex_math PUBLIC rex_diagnostics)
target_link_libraries(rex_math PUBLIC rex_utilities)

                                                                                                                             
# Set project properties                                                                                                       
set_target_properties(rex_math PROPERTIES FOLDER                                         1_foundations/core)  
set_target_properties(rex_math PROPERTIES DEFINE_SYMBOL                                  "" )                    
IF(MSVC AND REX_UNITY_BUILD)                                                                                                  
	set_target_properties(rex_math PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY                ${BIN_DIR})               
	set_target_properties(rex_math PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                     ${BIN_DIR})               
	set_target_properties(rex_math PROPERTIES LIBRARY_OUTPUT_DIRECTORY                     ${BIN_DIR})               
	set_target_properties(rex_math PROPERTIES RUNTIME_OUTPUT_DIRECTORY                     ${BIN_DIR})               

                                                                                                                             
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_EnableUnitySupport                 True)                     
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_IncludeInUnityFile                 True)                     
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_OrderInUnityFile                   100)                      
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_CombineFilesOnlyFromTheSameFolder  false)                    
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_MinFilesInUnityFile                2)                        
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_MaxFilesInUnityFile                0)                        
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_MinUnityFiles                      1)                        
	set_target_properties(rex_math PROPERTIES VS_GLOBAL_UnityFilesDirectory                .)                        
ENDIF()


# Compiler options
IF(MSVC)
	target_compile_options(rex_math PRIVATE /W4 /WX /MP)
ENDIF()


# Set precompiled header
add_precompiled_header(rex_math "rex_math_pch.h" "rex_math_pch.cpp")


