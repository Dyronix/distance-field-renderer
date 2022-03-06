

# This file was automatically generated by "create_project.py"
# Command: generate_project(type project_name solution_folder project_folder pch_header_name = project_folder pch_src_name = project_folder)
# Example: generate_project(static CoreLibs 1_common core_libs)


# include path: ${CMAKE_SOURCE_DIR}/source/include/2_platform/rex_windows
# src path: ${CMAKE_SOURCE_DIR}/source/src/2_platform/rex_windows


# Project Name Project
# -------------------------
file(GLOB_RECURSE rex_windows_LIBS_INC    ${CMAKE_SOURCE_DIR}/source/include/2_platform/rex_windows/*.h)
file(GLOB_RECURSE rex_windows_LIBS_SRC    ${CMAKE_SOURCE_DIR}/source/src/2_platform/rex_windows/*.cpp; ${CMAKE_SOURCE_DIR}/source/src/2_platform/rex_windows/*.c)


# Create the project filters
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/include/2_platform/rex_windows include)
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/src/2_platform/rex_windows src)


# Create the project
add_library(rex_windows STATIC ${rex_windows_LIBS_INC} ${rex_windows_LIBS_SRC})

STRING(TOUPPER rex_windows UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)


# Set the include directories                                                                                                  
target_include_directories(rex_windows PUBLIC ${CMAKE_SOURCE_DIR}/source/include/2_platform/rex_windows)                
target_include_directories(rex_windows PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_utilities)
target_include_directories(rex_windows PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_diagnostics)
target_include_directories(rex_windows PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_glad)
target_include_directories(rex_windows PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_core)
target_include_directories(rex_windows PUBLIC ${INCLUDE_GSL})
target_include_directories(rex_windows PUBLIC ${INCLUDE_SDL2})

# Set target link libraries
target_link_libraries(rex_windows PUBLIC rex_utilities)
target_link_libraries(rex_windows PUBLIC rex_diagnostics)
target_link_libraries(rex_windows PUBLIC rex_glad)
target_link_libraries(rex_windows PUBLIC rex_core)
target_link_libraries(rex_windows PUBLIC opengl32)
target_link_libraries(rex_windows PUBLIC ${LIB_SDL2})
target_link_libraries(rex_windows PUBLIC ${LIB_SDL2_MAIN})
                                                                                                                             
# Set project properties                                                                                                       
set_target_properties(rex_windows PROPERTIES FOLDER                                         2_platform)  
set_target_properties(rex_windows PROPERTIES DEFINE_SYMBOL                                  "" )                    
IF(MSVC AND REX_UNITY_BUILD)                                                                                                  
	set_target_properties(rex_windows PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY                ${BIN_DIR})               
	set_target_properties(rex_windows PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                     ${BIN_DIR})               
	set_target_properties(rex_windows PROPERTIES LIBRARY_OUTPUT_DIRECTORY                     ${BIN_DIR})               
	set_target_properties(rex_windows PROPERTIES RUNTIME_OUTPUT_DIRECTORY                     ${BIN_DIR})               

                                                                                                                             
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_EnableUnitySupport                 True)                     
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_IncludeInUnityFile                 True)                     
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_OrderInUnityFile                   100)                      
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_CombineFilesOnlyFromTheSameFolder  false)                    
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_MinFilesInUnityFile                2)                        
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_MaxFilesInUnityFile                0)                        
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_MinUnityFiles                      1)                        
	set_target_properties(rex_windows PROPERTIES VS_GLOBAL_UnityFilesDirectory                .)                        
ENDIF()


# Compiler options
IF(MSVC)
	target_compile_options(rex_windows PRIVATE /W4 /WX /MP)
ENDIF()


# Set precompiled header
add_precompiled_header(rex_windows "rex_windows_pch.h" "rex_windows_pch.cpp")


