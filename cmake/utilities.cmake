##########################################################################################################
# Macro that will define our source groups
##########################################################################################################
MACRO(GROUPSOURCES curdir)
    FILE(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR}/${curdir} ${CMAKE_CURRENT_LIST_DIR}/${curdir}/*)
    FOREACH(child ${children})
           IF(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${curdir}/${child})
              GROUPSOURCES(${curdir}/${child})
           ELSE()
              STRING(REPLACE "/" "\\" groupname ${curdir})
              STRING(REPLACE "." "${SRC_DIR}" groupname ${groupname})
              SOURCE_GROUP(${groupname} FILES ${CMAKE_CURRENT_LIST_DIR}/${curdir}/${child})
           ENDIF()
    ENDFOREACH()
ENDMACRO(GROUPSOURCES)
