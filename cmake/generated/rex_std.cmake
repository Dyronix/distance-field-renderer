

# This file was automatically generated by "create_project.py"
# Command: generate_project(type project_name solution_folder project_folder pch_header_name = project_folder pch_src_name = project_folder)
# Example: generate_project(static CoreLibs 1_common core_libs)


# include path: ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_std
# src path: ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_std


# Project Name Project
# -------------------------
file(GLOB_RECURSE rex_std_LIBS_INC    ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_std/*.h)
file(GLOB_RECURSE rex_std_LIBS_SRC    ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_std/*.cpp; ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_std/*.c)


# Create the project filters
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_std include)
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_std src)


# Create the project
add_library(rex_std STATIC ${rex_std_LIBS_INC} ${rex_std_LIBS_SRC})

STRING(TOUPPER rex_std UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)


# Set the include directories                                                                                                  
target_include_directories(rex_std PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_std)                
target_include_directories(rex_std PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_diagnostics)                
target_include_directories(rex_std PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_rex_utilities)              
target_include_directories(rex_std PUBLIC ${INCLUDE_GSL})  
target_include_directories(rex_std PUBLIC ${INCLUDE_CRC})  

# Set target link libraries
target_link_libraries(rex_std PUBLIC rex_utilities)
target_link_libraries(rex_std PUBLIC rex_diagnostics)
                                                                                                                             
# Set project properties                                                                                                       
set_target_properties(rex_std PROPERTIES FOLDER                                         1_foundations/core)  
set_target_properties(rex_std PROPERTIES DEFINE_SYMBOL                                  "" )                    
IF(MSVC AND REX_UNITY_BUILD)                                                                                                  
	set_target_properties(rex_std PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY                ${BIN_DIR})               
	set_target_properties(rex_std PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                     ${BIN_DIR})               
	set_target_properties(rex_std PROPERTIES LIBRARY_OUTPUT_DIRECTORY                     ${BIN_DIR})               
	set_target_properties(rex_std PROPERTIES RUNTIME_OUTPUT_DIRECTORY                     ${BIN_DIR})               

                                                                                                                             
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_EnableUnitySupport                 True)                     
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_IncludeInUnityFile                 True)                     
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_OrderInUnityFile                   100)                      
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_CombineFilesOnlyFromTheSameFolder  false)                    
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_MinFilesInUnityFile                2)                        
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_MaxFilesInUnityFile                0)                        
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_MinUnityFiles                      1)                        
	set_target_properties(rex_std PROPERTIES VS_GLOBAL_UnityFilesDirectory                .)                        
ENDIF()


# Compiler options
IF(MSVC)
	target_compile_options(rex_std PRIVATE /W4 /WX /MP)
ENDIF()


# Set precompiled header
add_precompiled_header(rex_std "rex_std_pch.h" "rex_std_pch.cpp")


