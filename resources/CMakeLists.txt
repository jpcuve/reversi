add_custom_target(resources DEPENDS resources.res)

add_custom_command(
        OUTPUT resources.res
        COMMAND "${WINDOWS_KIT}/bin/${WINDOWS_SDK_VERSION}/${WINDOWS_ARCHITECTURE}/rc.exe" -fo resources.res ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc
        DEPENDS resources.rc test.txt
)

