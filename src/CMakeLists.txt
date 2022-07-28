set(EXT_DIR "../external")

set(LIBS sdl2 opengl32)
set(INCLUDE_DIRS ${EXT_DIR} ${EXT_DIR}/glm)

add_executable(bootstrap main.cpp)
target_link_libraries(bootstrap ${LIBS})
include_directories(${INCLUDE_DIRS})