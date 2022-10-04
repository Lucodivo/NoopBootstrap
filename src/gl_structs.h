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

struct ShaderProgram {
  GLuint id;
  GLuint vertexShader;
  GLuint fragmentShader;
  const char* vertexFileName;
  const char* fragmentFileName;
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

u32 posUBOBindingIndex = 1;
struct PosUBO {                 // base alignment   // aligned offset
  glm::vec2 spriteDimens;       // 8                // 0
  glm::vec2 emulatedWindowRes;  // 8                // 8
  glm::vec2 pos;                // 8                // 16
};

u32 debugUBOBindingIndex = 2;
struct DebugUBO {               // base alignment   // aligned offset
  glm::vec4 debugColor;         // 16                // 0
};