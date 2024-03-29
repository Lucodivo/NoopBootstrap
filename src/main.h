#pragma once

#include <SDL.h>
#include <SDL_audio.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "tinygltf/tiny_gltf.h"

#include <cassert>
#include <iostream>

#define GLM_FORCE_LEFT_HANDED
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "types.h"
#include "platform.h"
#include "util.h"
#include "platform.cpp"
#include "gl_structs.h"
#include "gl_util.h"
#include "texture.h"
#include "model.h"
#include "shader_program.h"
#include "simple_vertex_atts.h"
const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
#include "camera.h"

#define OUT