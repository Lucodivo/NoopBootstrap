cmake_minimum_required(VERSION 3.8)
project(bootstrap_dependencies)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build)
set(EXT_DIR ${CMAKE_SOURCE_DIR})
include_directories(${EXT_DIR})

# SDL2
add_subdirectory(sdl2)

# glad
add_library(glad STATIC glad/glad.c)
target_include_directories(glad PUBLIC ${EXT_DIR}/glad)

# stb
add_library(stb STATIC stb/stb.c stb/stb_image.h stb/stb_image_write.h)

# tiny_gltf
add_library(tinygltf STATIC tinygltf/tiny_gltf.cpp tinygltf/tiny_gltf.h)
target_include_directories(tinygltf PUBLIC nlohmann/ stb/)

# Imgui
file(GLOB IMGUI_SOURCE_FILES ${EXT_DIR}/imgui/*.cpp ${EXT_DIR}/imgui/*.h
        ${EXT_DIR}/imgui/backends/imgui_impl_sdl.h ${EXT_DIR}/imgui/backends/imgui_impl_sdl.cpp
        ${EXT_DIR}/imgui/backends/imgui_impl_opengl3.h ${EXT_DIR}/imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${IMGUI_SOURCE_FILES})
target_include_directories(imgui PUBLIC ${EXT_DIR}/imgui ${EXT_DIR}/sdl2/include)

add_custom_target(
        bootstrap_dependencies
        DEPENDS SDL2 SDL2main glad imgui stb tinygltf
)