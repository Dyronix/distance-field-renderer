# -------------------------------
# Directories       
# -------------------------------
SET (SCRIPTS_DIR ${PROJECT_SOURCE_DIR}\\scripts)
SET (SRC_DIR ${PROJECT_SOURCE_DIR}\\source)
SET (EXTERNALS_DIR ${PROJECT_SOURCE_DIR}\\extern)
SET (MODULES_DIR ${SRC_DIR}\\modules)
SET (BIN_DIR ${PROJECT_BINARY_DIR}\\bin)

set(LIB_SDL2 ${EXTERNALS_DIR}/sdl-windows/lib/x64/SDL2.lib)
set(LIB_SDL2_MAIN ${EXTERNALS_DIR}/sdl-windows/lib/x64/SDL2main.lib)

set(INCLUDE_SDL2 ${EXTERNALS_DIR}/sdl-windows/include)