#pragma once

// source: Naming A Texture Object in The Official Guide to Learning OpenGL, Version 1.1
#define TEXTURE_ID_NO_TEXTURE 0

/* Handles */
struct VertexAtt {
  GLuint arrayObject;
  GLuint bufferObject;
  GLuint indexObject;
  GLsizei indexCount;
  u32 indexTypeSizeInBytes;
};

enum TextureType {
  Albedo,
  Normal,
};

struct Shader {
  GLuint id;
  const char* fileName
};

struct FragmentShader: public Shader {
  GLuint
};

struct ShaderProgram {
  GLuint id;
//  Shader vertexShader;
//  Shader fragmentShader;
};

struct Mesh {
  VertexAtt vertexAtt;
  GLuint albedoTextureId;
  GLuint normalTextureId;
  glm::vec4 baseColor;
};

struct Model {
  Mesh* meshes;
  u32 meshCount;
  Box boundingBox;
  const char* fileName;
};

/* Uniform buffer objects */
u32 modelViewProjUBOBindingIndex = 0;
struct ModelViewProjUBO {  // base alignment   // aligned offset
  glm::mat4 model;         // 16                // 0
  glm::mat4 view;          // 16                // 64
  glm::mat4 proj;          // 16                // 128
};

u32 pixelUBOBindingIndex = 1;
struct PixelUBO {                 // base alignment   // aligned offset
  ivec2 spriteDimens;       // 8                // 0
  ivec2 emulatedWindowRes;  // 8                // 8
  glm::vec2 pos;                // 8                // 16
};

u32 debugUBOBindingIndex = 2;
struct DebugUBO {               // base alignment   // aligned offset
  glm::vec4 debugColor;         // 16                // 0
};

enum UniformBufferUsage {
  FrameDynamic = GL_DYNAMIC_DRAW,
  Static = GL_STATIC_DRAW
};

GLuint createUniformBuffer(std::size_t bufferSize, UniformBufferUsage usage) {
  GLuint glBufferId;
  glGenBuffers(1, &glBufferId);
  // - allocate memory for view-model-proj buffer
  glBindBuffer(GL_UNIFORM_BUFFER, glBufferId);
  glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, usage);
  return glBufferId;
}

void setUniformData(GLuint glBufferId, std::size_t offset, std::size_t size, const void* data) {
  glBindBuffer(GL_UNIFORM_BUFFER, glBufferId);
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

struct ModelViewProjUniformBuffer {
  GLuint glBufferId;

  ModelViewProjUniformBuffer() { glBufferId = createUniformBuffer(sizeof(ModelViewProjUBO), UniformBufferUsage::FrameDynamic); bind(); }
  ~ModelViewProjUniformBuffer() { glDeleteBuffers(1, &glBufferId); }

  void bind() { glBindBufferBase(GL_UNIFORM_BUFFER, modelViewProjUBOBindingIndex, glBufferId); } // "binding = 0" in shaders
  void setProjection(glm::mat4* projMat) { setUniformData(glBufferId, offsetof(ModelViewProjUBO, proj), sizeof(glm::mat4), projMat); }
  void setView(glm::mat4* viewMat) { setUniformData(glBufferId, offsetof(ModelViewProjUBO, view), sizeof(glm::mat4), viewMat); }
  void setModel(glm::mat4* modelMat) { setUniformData(glBufferId, offsetof(ModelViewProjUBO, model), sizeof(glm::mat4), modelMat); }
};

struct PixelUniformBuffer {
  GLuint glBufferId;

  PixelUniformBuffer() { glBufferId = createUniformBuffer(sizeof(PixelUBO), UniformBufferUsage::FrameDynamic); bind(); }
  ~PixelUniformBuffer() { glDeleteBuffers(1, &glBufferId); }

  void bind() { glBindBufferBase(GL_UNIFORM_BUFFER, pixelUBOBindingIndex, glBufferId); } // "binding = 0" in shaders
  void setSpriteDimens(ivec2* spriteDimens) { setUniformData(glBufferId, offsetof(PixelUBO, spriteDimens), sizeof(ivec2), spriteDimens); }
  void setEmulatedWindowRes(ivec2* emulatedWindowRes) { setUniformData(glBufferId, offsetof(PixelUBO, emulatedWindowRes), sizeof(ivec2), emulatedWindowRes); }
  void setPos(glm::vec2* pos) { setUniformData(glBufferId, offsetof(PixelUBO, pos), sizeof(glm::vec2), pos); }
};

struct DebugUniformBuffer {
  GLuint glBufferId;

  DebugUniformBuffer() { glBufferId = createUniformBuffer(sizeof(DebugUBO), UniformBufferUsage::FrameDynamic); bind(); }
  ~DebugUniformBuffer() { glDeleteBuffers(1, &glBufferId); }

  void bind() { glBindBufferBase(GL_UNIFORM_BUFFER, debugUBOBindingIndex, glBufferId); } // "binding = 0" in shaders
  void setDebugColor(const glm::vec4* debugColor) { setUniformData(glBufferId, offsetof(DebugUBO, debugColor), sizeof(glm::vec4), debugColor); }
};