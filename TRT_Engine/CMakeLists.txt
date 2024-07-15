cmake_minimum_required(VERSION 3.27)
project(TRT_ENGINE LANGUAGES C)

file(GLOB SOURCES *.c)
file(GLOB HEADERS *.h)

add_library(TRT_ENGINE ${SOURCES} ${HEADERS})
target_include_directories(TRT_ENGINE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})