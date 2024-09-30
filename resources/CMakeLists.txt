add_custom_target(resources DEPENDS resources.res)

find_program(RC rc DOC "resource compiler")

add_custom_command(
        OUTPUT resources.res
        COMMAND ${RC} -fo resources.res ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc
        DEPENDS resources.rc test.txt
)

