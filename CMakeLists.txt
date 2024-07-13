cmake_minimum_required(VERSION 3.27)
project(Wolfenstein3D LANGUAGES C)

file(GLOB SOURCES *.c)
file(GLOB HEADERS *.h)

file(GLOB ICON resources/icon/app.o)
file(GLOB RESOURCE resources/Resources.rc)

file(GLOB STB_IMAGE vendor/stb/*)

add_executable(Wolfenstein3D WIN32 ${ICON} ${SOURCES} ${HEADERS} ${STB_IMAGE} ${RESOURCE})

find_library(WINMM_LIB NAMES winmm)
target_link_libraries(Wolfenstein3D ${WINMM_LIB})

set_target_properties(Wolfenstein3D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)