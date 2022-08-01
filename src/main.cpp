#include <SDL.h>
#include <glad/glad.h>

#include <cassert>
#include <iostream>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "types.h"
#include "util.h"

#include "platform.h"
#include "gl_structs.h"
#include "gl_util.h"
#include "graphics_util.h"
#include "shader_program.h"
#include "cube.h"

#define INIT_WINDOW_WIDTH 640
#define INIT_WINDOW_HEIGHT 480
#define INIT_ASPECT (f32)INIT_WINDOW_WIDTH / INIT_WINDOW_HEIGHT

int main(int argc, char* argv[]) {
  WINDOW_HANDLE window = initWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
  loadOpenGL();

  const glm::vec3 worldUp = glm::vec3{0.0f, 1.0f, 0.0f};

  VertexAtt cubeVertAtt = initCubePosVertexAttBuffers();
  glm::vec3 cubeColor = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 cubePosition = glm::vec3{0.0f, 0.0f, 0.0f};
  glm::vec3 cubeScale = glm::vec3(1.5f);
  glm::vec3 cubeInitRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 cubeActiveRotationAxis = worldUp;
  f32 cubeActiveRotationPerSecond = 20.0f * RadiansPerDegree;
  glm::mat4 cubeScaleRotationMat = glm::rotate(glm::scale(glm::mat4(), cubeScale), 45.0f * RadiansPerDegree, cubeInitRotationAxis);
  glm::mat4 cubeTranslationMat = glm::translate(glm::mat4(), cubePosition);

  glm::vec3 clearColor = glm::vec3{0.5f, 0.0f, 0.0f};
  glm::vec3 cameraPosition = glm::vec3{0.0f, 0.0f, -5.0f};
  glm::mat4 viewMat = glm::lookAt(cameraPosition, cubePosition, worldUp);
  glm::mat4 projMat = glm::perspective(fieldOfView(13.5f, 25.0f), INIT_ASPECT, 0.01f, 100.0f);

  ShaderProgram shaderProgram = createShaderProgram("shaders/pos.vert", "shaders/single_color.frag");
  glUseProgram(shaderProgram.id);
  setUniform(shaderProgram.id, "singleColor", cubeColor);
  setUniform(shaderProgram.id, "view", viewMat);
  setUniform(shaderProgram.id, "projection", projMat);

  glBindVertexArray(cubeVertAtt.arrayObject);

  InputState inputState = {};
  while(!inputState.quit) {
    getKeyboardInput(&inputState);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 cubeFrameScaleRotationMat = glm::rotate(cubeScaleRotationMat, static_cast<f32>(cubeActiveRotationPerSecond * getElapsedTime()), cubeActiveRotationAxis);
    setUniform(shaderProgram.id, "model", cubeTranslationMat * cubeFrameScaleRotationMat);

    glDrawElements(GL_TRIANGLES, // drawing mode
                   cubeVertAtt.indexCount, // number of elements
                   glSizeInBytes(cubeVertAtt.indexTypeSizeInBytes), // type of the indices
                   0); // offset in the EBO

    swapBuffers(window);
  }

  deinitWindow(window);

  return 0;
}