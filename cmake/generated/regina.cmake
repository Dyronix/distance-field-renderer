

# This file was automatically generated by "create_project.py"
# Command: generate_project(type project_name solution_folder project_folder pch_header_name = project_folder pch_src_name = project_folder)
# Example: generate_project(static CoreLibs 1_common core_libs)


# include path: ${CMAKE_SOURCE_DIR}/source/include/3_tools/regina
# src path: ${CMAKE_SOURCE_DIR}/source/src/3_tools/regina


# Project Name Project
# -------------------------
file(GLOB_RECURSE regina_LIBS_INC    ${CMAKE_SOURCE_DIR}/source/include/3_tools/regina/*.h)
file(GLOB_RECURSE regina_LIBS_SRC    ${CMAKE_SOURCE_DIR}/source/src/3_tools/regina/*.cpp, ${CMAKE_SOURCE_DIR}/source/src/3_tools/regina/*.c)


# Create the project filters
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/include/3_tools/regina include)
GROUPSOURCES(${CMAKE_SOURCE_DIR}/source/src/3_tools/regina src)


# Create the project
add_executable(regina EXECUTABLE ${regina_LIBS_INC} ${regina_LIBS_SRC})


# Set the include directories
target_include_directories(regina PUBLIC ${CMAKE_SOURCE_DIR}/source/include/3_tools/regina)

# Set the link libraries
target_link_libraries(regina PUBLIC rex_core)
target_link_libraries(regina PUBLIC opengl32)
target_link_libraries(regina PUBLIC ${LIB_SDL2})
target_link_libraries(regina PUBLIC ${LIB_SDL2_MAIN})

# Set project properties
set_target_properties(regina PROPERTIES FOLDER                                         3_tools) 
set_target_properties(regina PROPERTIES DEFINE_SYMBOL                                  "" )                   
IF(MSVC)
	set_target_properties(regina PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY                ${OUTPUT_BINDIR})        
	set_target_properties(regina PROPERTIES ARCHIVE_OUTPUT_DIRECTORY                     ${OUTPUT_BINDIR})        
	set_target_properties(regina PROPERTIES LIBRARY_OUTPUT_DIRECTORY                     ${OUTPUT_BINDIR})        
	set_target_properties(regina PROPERTIES RUNTIME_OUTPUT_DIRECTORY                     ${OUTPUT_BINDIR})        


	set_target_properties(regina PROPERTIES VS_GLOBAL_EnableUnitySupport                 True)                    
	set_target_properties(regina PROPERTIES VS_GLOBAL_IncludeInUnityFile                 True)
	set_target_properties(regina PROPERTIES VS_GLOBAL_OrderInUnityFile                   100)
	set_target_properties(regina PROPERTIES VS_GLOBAL_CombineFilesOnlyFromTheSameFolder  false)
	set_target_properties(regina PROPERTIES VS_GLOBAL_MinFilesInUnityFile                2)
	set_target_properties(regina PROPERTIES VS_GLOBAL_MaxFilesInUnityFile                0)
	set_target_properties(regina PROPERTIES VS_GLOBAL_MinUnityFiles                      1)
	set_target_properties(regina PROPERTIES VS_GLOBAL_UnityFilesDirectory                .)
ENDIF()


# Compiler options
IF(MSVC)
	target_compile_options(regina PRIVATE /W4 /WX /MP)
ENDIF()


# Set precompiled header
add_precompiled_header(regina "regina_pch.h" "regina_pch.cpp")


# Execute post build command for regina
add_custom_command
(
    TARGET regina
    POST_BUILD
    WORKING_DIRECTORY ${SCRIPTS_DIR}/${LOWER_PLATFORM_TARGET}
    COMMAND PowerShell -File cmake-post-build.ps1
)
