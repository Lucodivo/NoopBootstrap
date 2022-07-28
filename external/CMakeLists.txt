add_subdirectory(sdl2)

# SDL2
add_library(sdl2 INTERFACE)
add_dependencies(sdl2 SDL2 SDL2main)
set(sdl2_DIR "sdl2")
target_link_libraries(sdl2 INTERFACE SDL2d SDL2maind)

target_include_directories(sdl2 INTERFACE ${sdl2_DIR}/include ${sdl2_image_DIR}/include)
target_link_directories(sdl2 INTERFACE ${sdl2_DIR}/build/Debug/ ${sdl2_image_DIR}/build/Debug)