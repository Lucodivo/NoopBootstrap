#pragma once

struct VertexAtt {
  GLuint arrayObject;
  GLuint bufferObject;
  GLuint indexObject;
  u32 indexCount;
  u32 indexTypeSizeInBytes;
};

struct ShaderProgram {
  GLuint id;
  GLuint vertexShader;
  GLuint fragmentShader;
  const char* vertexFileName;
  const char* fragmentFileName;
};