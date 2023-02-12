cmake_minimum_required(VERSION 3.8)
project(bootstrap)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS) # Remove errors similar to the following "ERROR: printf should be "printf_s"
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # Where executables and dynamic libraries go
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build) # Where static libraries go
set(ARCHIVE_LIB_HINTS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Release)
set(EXT_DIR "${PROJECT_SOURCE_DIR}/external")
set(INCLUDE_DIRS ${EXT_DIR} ${EXT_DIR}/glm ${EXT_DIR}/sdl2/include ${EXT_DIR}/glad ${EXT_DIR}/imgui ${EXT_DIR}/imgui/backends)

include_directories(${INCLUDE_DIRS})

# Find static libraries
# These static libraries are built by this project and should be pulled from nowhere else
find_library(glad NAMES glad PATHS ${ARCHIVE_LIB_HINTS} NO_DEFAULT_PATH REQUIRED)
find_library(imgui NAMES imgui PATHS ${ARCHIVE_LIB_HINTS} NO_DEFAULT_PATH REQUIRED)
find_library(stb NAMES stb PATHS ${ARCHIVE_LIB_HINTS} NO_DEFAULT_PATH REQUIRED)
find_library(tinygltf NAMES tinygltf PATHS ${ARCHIVE_LIB_HINTS} NO_DEFAULT_PATH REQUIRED)
find_library(sdl2-lib NAMES SDL2 SDL2d PATHS ${ARCHIVE_LIB_HINTS} NO_DEFAULT_PATH REQUIRED)
find_library(sdl2main-lib NAMES SDL2main SDL2maind PATHS ${ARCHIVE_LIB_HINTS} NO_DEFAULT_PATH REQUIRED)

#SDL2
add_library(sdl2 INTERFACE)
target_link_libraries(sdl2 INTERFACE ${sdl2-lib} ${sdl2main-lib})
target_link_directories(sdl2 INTERFACE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
target_include_directories(sdl2 INTERFACE ${EXT_DIR}/sdl2/include)
set_target_properties(sdl2 PROPERTIES DEBUG_POSTFIX "d")

add_subdirectory(src)