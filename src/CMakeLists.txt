set(LIBS sdl2)
set(INCLUDE_DIRS)

add_executable(bootstrap main.cpp)
target_link_libraries(bootstrap ${LIBS})
include_directories(${INCLUDE_DIRS})