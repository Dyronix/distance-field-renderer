

# This file was automatically generated by "create_project.py"
# Command: generate_project(type project_name solution_folder project_folder pch_header_name = project_folder pch_src_name = project_folder)
# Example: generate_project(static CoreLibs 1_common core_libs)


# include path: ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_scenegraph
# src path: ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_scenegraph


# Project Name Project
# -------------------------
file(GLOB_RECURSE rex_scenegraph_LIBS_INC    ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_scenegraph/*.h)
file(GLOB_RECURSE rex_scenegraph_LIBS_SRC    ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_scenegraph/*.cpp; ${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_scenegraph/*.c)


# Create the project filters
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_scenegraph include)
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/src/1_foundations/rex_scenegraph src)


# Create the project
add_library(rex_scenegraph STATIC ${rex_scenegraph_LIBS_INC} ${rex_scenegraph_LIBS_SRC})

STRING(TOUPPER rex_scenegraph UPPER_LIB_NAME)
add_definitions(-D${UPPER_LIB_NAME}_LIB)


# Set the include directories                                                                                                  
target_include_directories(rex_scenegraph PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_scenegraph)                
target_include_directories(rex_scenegraph PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_diagnostics)
target_include_directories(rex_scenegraph PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_utilities)
target_include_directories(rex_scenegraph PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_std)
target_include_directories(rex_scenegraph PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_math)
target_include_directories(rex_scenegraph PUBLIC ${CMAKE_SOURCE_DIR}/source/include/1_foundations/rex_graphics)
target_include_directories(rex_scenegraph PUBLIC ${INCLUDE_ENTT})

# Set target link libraries
target_link_libraries(rex_scenegraph PUBLIC rex_diagnostics)
target_link_libraries(rex_scenegraph PUBLIC rex_utilities)
target_link_libraries(rex_scenegraph PUBLIC rex_std)
target_link_libraries(rex_scenegraph PUBLIC rex_math)
target_link_libraries(rex_scenegraph PUBLIC rex_graphics)
                                                                                                                             
# Set project properties                                                                                                       
set_target_properties(rex_scenegraph PROPERTIES FOLDER                                         1_foundations/engine)  
set_target_properties(rex_scenegraph PROPERTIES DEFINE_SYMBOL                                  "" )                    
IF(MSVC)
	set_property(TARGET rex_scenegraph PROPERTY VS_DEBUGGER_WORKING_DIRECTORY                	 ${BIN_DIR})        

	set_target_properties(rex_scenegraph PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                     ${BIN_DIR})        
	set_target_properties(rex_scenegraph PROPERTIES LIBRARY_OUTPUT_DIRECTORY                     ${BIN_DIR})        
	set_target_properties(rex_scenegraph PROPERTIES RUNTIME_OUTPUT_DIRECTORY                     ${BIN_DIR})        

	IF(REX_UNITY_BUILD)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_EnableUnitySupport                 True)                    
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_IncludeInUnityFile                 True)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_OrderInUnityFile                   100)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_CombineFilesOnlyFromTheSameFolder  false)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_MinFilesInUnityFile                2)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_MaxFilesInUnityFile                0)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_MinUnityFiles                      1)
		set_target_properties(rex_scenegraph PROPERTIES VS_GLOBAL_UnityFilesDirectory                .)
	ENDIF()
	
ENDIF()


# Compiler options
IF(MSVC)
	target_compile_options(rex_scenegraph PRIVATE /W4 /WX /MP)
ENDIF()


# Set precompiled header
add_precompiled_header(rex_scenegraph "rex_scenegraph_pch.h" "rex_scenegraph_pch.cpp")


