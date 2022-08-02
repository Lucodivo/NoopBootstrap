add_executable(bootstrap main.cpp)
set(LIBS opengl32 ${glad} sdl2 ${imgui})
target_link_libraries(bootstrap ${LIBS})