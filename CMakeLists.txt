cmake_minimum_required(VERSION 3.27)
project(Wolfenstein3D LANGUAGES C)

add_subdirectory(TRT_Engine)

file(GLOB SOURCES *.c)
file(GLOB HEADERS *.h)

if(MSVC)
    set(RES_COMPILER rc)
else()
    set(RES_COMPILER windres)
endif()

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.o
        COMMAND ${RES_COMPILER} -I${CMAKE_SOURCE_DIR}/resources -i${CMAKE_SOURCE_DIR}/resources/icon/icon.rc -o ${CMAKE_CURRENT_BINARY_DIR}/icon.o
        DEPENDS ${CMAKE_SOURCE_DIR}/resources/icon/icon.rc
)

file(GLOB ICON ${CMAKE_CURRENT_BINARY_DIR}/icon.o)
file(GLOB RESOURCE resources/icon/icon.rc)

file(GLOB STB_IMAGE vendor/stb/*)

add_executable(Wolfenstein3D WIN32 ${ICON} ${SOURCES} ${HEADERS} ${STB_IMAGE} ${RESOURCE})

find_library(WINMM_LIB NAMES winmm)
target_link_libraries(Wolfenstein3D ${WINMM_LIB} TRT_ENGINE)

set_target_properties(Wolfenstein3D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)