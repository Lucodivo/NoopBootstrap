cmake_minimum_required(VERSION 3.8)
project(bootstrap_dependencies)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build)
set(EXT_DIR ${CMAKE_SOURCE_DIR})
include_directories(${EXT_DIR})

add_subdirectory(sdl2)

add_library(glad STATIC glad/glad.c)
target_include_directories(glad PUBLIC ${EXT_DIR}/glad)

add_custom_target(
        bootstrap_dependencies
        DEPENDS SDL2 SDL2main glad
)