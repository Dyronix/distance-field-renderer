# -------------------------------
# Directories       
# -------------------------------
SET (SCRIPTS_DIR ${CMAKE_SOURCE_DIR}\\scripts)
SET (SRC_DIR ${CMAKE_SOURCE_DIR}\\source)
SET (EXTERNALS_DIR ${CMAKE_SOURCE_DIR}\\external)
SET (MODULES_DIR ${SRC_DIR}\\modules)
SET (BIN_DIR ${CMAKE_BINARY_DIR}\\bin)

IF(${PLATFORM_TARGET} STREQUAL ${WINDOWS})
    set(LIB_SDL2 ${EXTERNALS_DIR}/sdl-windows/lib/x64/SDL2.lib)
    set(LIB_SDL2_MAIN ${EXTERNALS_DIR}/sdl-windows/lib/x64/SDL2main.lib)
    
    set(INCLUDE_SDL2 ${EXTERNALS_DIR}/sdl-windows/include)
ENDIF()

set(INCLUDE_SPDLOG ${EXTERNALS_DIR}/spdlog-windows/include)