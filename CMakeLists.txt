cmake_minimum_required(VERSION 3.8)
project(bootstrap)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(ARCHIVE_LIB_HINTS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release)
set(EXT_DIR "${PROJECT_SOURCE_DIR}/external")

# glad
find_library(
        glad
        NAMES glad
        HINTS ${ARCHIVE_LIB_HINTS}
        REQUIRED
)

# glad
find_library(
        imgui
        NAMES imgui
        HINTS ${ARCHIVE_LIB_HINTS}
        REQUIRED
)

#SDL2
find_library(
        sdl2-lib
        NAMES SDL2 SDL2d
        HINTS ${ARCHIVE_LIB_HINTS}
        REQUIRED
)
find_library(
        sdl2main-lib
        NAMES SDL2main SDL2maind
        HINTS ${ARCHIVE_LIB_HINTS}
        REQUIRED
)
add_library(sdl2 INTERFACE)
set(sdl2_DIR ${PROJECT_SOURCE_DIR}/bin/debug)
target_link_libraries(sdl2 INTERFACE ${sdl2-lib} ${sdl2main-lib})
target_include_directories(sdl2 INTERFACE ${EXT_DIR}/sdl2/include)
target_link_directories(sdl2 INTERFACE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(INCLUDE_DIRS ${EXT_DIR} ${EXT_DIR}/glm ${EXT_DIR}/sdl2/include ${EXT_DIR}/glad ${EXT_DIR}/imgui ${EXT_DIR}/imgui/backends)
include_directories(${INCLUDE_DIRS})

add_subdirectory(src)