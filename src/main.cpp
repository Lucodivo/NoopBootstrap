#include <SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "nlohmann/json.hpp"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NOEXCEPTION
#include "tinygltf/tiny_gltf.h"

#include <cassert>
#include <iostream>

#define GLM_FORCE_LEFT_HANDED
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "types.h"
#include "math_structs.h"
#include "util.h"

#include "platform.h"
#include "gl_structs.h"
#include "gl_util.h"
#include "graphics_util.h"
#include "texture.h"
#include "model.h"
#include "shader_program.h"
#include "simple_vertex_data.h"

#define INIT_WINDOW_WIDTH 640
#define INIT_WINDOW_HEIGHT 480
#define INIT_ASPECT (f32)INIT_WINDOW_WIDTH / INIT_WINDOW_HEIGHT

void scene(WINDOW_HANDLE windowHandle);

int main(int argc, char* argv[]) {
  WINDOW_HANDLE windowHandle;
  GL_CONTEXT_HANDLE glContextHandle;
  initWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, &windowHandle, &glContextHandle);
  loadOpenGL();
  initImgui(windowHandle, glContextHandle);
  scene(windowHandle);
  deinitWindow(windowHandle);
  return 0;
}

void scene(WINDOW_HANDLE windowHandle) {

  const glm::vec3 worldUp = glm::vec3{0.0f, 1.0f, 0.0f};

  s32 windowWidth, windowHeight;
  windowExtent(windowHandle, &windowWidth, &windowHeight);

  GLuint spiritTexture, birdTexture;
  u32 spiritTexWidth, spiritTexHeight, birdTexWidth, birdTexHeight;
  load2DTexture("data/textures/seed_spirit.png", &spiritTexture, &spiritTexWidth, &spiritTexHeight);
  load2DTexture("data/textures/bird_guy.png", &birdTexture, &birdTexWidth, &birdTexHeight);

  VertexAtt cubeVertAtt = initializeCubePosNormTexVertexAttBuffers();
  glm::vec3 cubePosition = glm::vec3{0.0f, 0.0f, 0.0f};
  glm::vec3 cubeScale = glm::vec3(1.5f);
  glm::vec3 cubeInitRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 cubeActiveRotationAxis = worldUp;
  f32 cubeActiveRotationPerSecond = 20.0f * RadiansPerDegree;
  glm::mat4 cubeScaleRotationMat = glm::rotate(glm::scale(glm::mat4(), cubeScale),  -45.0f * RadiansPerDegree, cubeInitRotationAxis);
  glm::mat4 cubeTranslationMat = glm::translate(glm::mat4(), cubePosition);

  Model quadModel;
  loadModel("data/models/quad.glb", &quadModel);
  glm::vec3 quadPosition = glm::vec3{-0.8f, -0.55f, -2.0f};
  f32 qScale = 0.2f;
  glm::vec3 quadScale = glm::vec3(qScale);
  glm::mat4 quadScaleMat = glm::scale(glm::mat4(), quadScale);
  glm::mat4 quadTranslationMat = glm::translate(glm::mat4(), quadPosition);

  glm::vec3 clearColor = glm::vec3{0.8f, 0.8f, 0.8f};
  glm::vec3 cameraPosition = glm::vec3{0.0f, 0.0f, -5.0f};
  glm::mat4 viewMat = glm::lookAt(cameraPosition, glm::vec3(0.0f), worldUp);
  glm::mat4 projMat = glm::perspective(fieldOfView(13.5f, 25.0f), INIT_ASPECT, 0.01f, 100.0f);

  ShaderProgram shaderProgram = createShaderProgram("shaders/pos.vert", "shaders/single_color.frag");
  glUseProgram(shaderProgram.id);

  // gnereate an ID for view-model-proj buffer
  GLuint modelViewProjUboId;
  glGenBuffers(1, &modelViewProjUboId);
  // allocate memory for view-model-proj buffer
  glBindBuffer(GL_UNIFORM_BUFFER, modelViewProjUboId);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ModelViewProjUBO), NULL, GL_STREAM_DRAW);
  // bind the projection matrix
  glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, proj), sizeof(glm::mat4), &projMat);
  // attach buffer to correct binding point (as specified in shader. ex: "binding = 0")
  glBindBufferRange(GL_UNIFORM_BUFFER, modelViewProjUBOBindingIndex, modelViewProjUboId, 0, sizeof(ModelViewProjUBO));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glViewport(0, 0, windowWidth, windowHeight);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  setSampler2D(shaderProgram.id, "albedoTex", 0);

  InputState inputState = {};
  bool showDemoWindow = false;
  while(!inputState.quit) {
    getKeyboardInput(&inputState);

    newFrameImGui();
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("View"))
      {
        if (ImGui::MenuItem("Demo Window ", NULL)) {
          showDemoWindow = !showDemoWindow;
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
    if(showDemoWindow) {
      ImGui::ShowDemoWindow(&showDemoWindow);
    }

    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 cubeFrameModelMat = cubeTranslationMat * glm::rotate(cubeScaleRotationMat, static_cast<f32>(cubeActiveRotationPerSecond * getElapsedTime()), cubeActiveRotationAxis);

    glBindBuffer(GL_UNIFORM_BUFFER, modelViewProjUboId);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, model), sizeof(glm::mat4), &cubeFrameModelMat);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, view), sizeof(glm::mat4), &viewMat);

    glDisable(GL_CULL_FACE);
    bindActiveTexture2d(0, spiritTexture);
    drawTriangles(cubeVertAtt);
    glEnable(GL_CULL_FACE);

    glm::mat4 quadFrameModelMat = quadTranslationMat * quadScaleMat;

    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, model), sizeof(glm::mat4), &quadFrameModelMat);

    bindActiveTexture2d(0, birdTexture);
    drawModel(quadModel);

    renderImGui();

    swapBuffers(windowHandle);
  }
}