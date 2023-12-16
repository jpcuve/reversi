cmake_minimum_required(VERSION 3.26)
project(reversi)

set(CMAKE_CXX_STANDARD 23)

set(WINDOWS_KIT "C:/Program Files (x86)/Windows Kits/10")
set(WINDOWS_SDK_VERSION 10.0.22621.0)
set(WINDOWS_ARCHITECTURE x64)
link_directories("${WINDOWS_KIT}/Lib/${WINDOWS_SDK_VERSION}/um/${WINDOWS_ARCHITECTURE}")

add_subdirectory(src)
add_subdirectory(resources)

