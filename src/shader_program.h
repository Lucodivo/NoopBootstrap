#pragma once

internal u32 loadShader(const char* shaderPath, GLenum shaderType);

ShaderProgram createShaderProgram(const char* vertexPath, const char* fragmentPath, const char* noiseTexture = nullptr) {
  ShaderProgram shaderProgram{};
  shaderProgram.vertexFileName = vertexPath;
  shaderProgram.fragmentFileName = fragmentPath;
  shaderProgram.vertexShader = loadShader(shaderProgram.vertexFileName, GL_VERTEX_SHADER);
  shaderProgram.fragmentShader = loadShader(shaderProgram.fragmentFileName, GL_FRAGMENT_SHADER);

  // shader program
  shaderProgram.id = glCreateProgram(); // NOTE: returns 0 if error occurs when creating program
  glAttachShader(shaderProgram.id, shaderProgram.vertexShader);
  glAttachShader(shaderProgram.id, shaderProgram.fragmentShader);
  glLinkProgram(shaderProgram.id);

  s32 linkSuccess;
  glGetProgramiv(shaderProgram.id, GL_LINK_STATUS, &linkSuccess);
  if (!linkSuccess)
  {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram.id, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    exit(-1);
  }

  glDetachShader(shaderProgram.id, shaderProgram.vertexShader);
  glDetachShader(shaderProgram.id, shaderProgram.fragmentShader);

  return shaderProgram;
}

internal u32 loadShader(const char* shaderPath, GLenum shaderType) {
  std::string shaderTypeStr;
  if(shaderType == GL_VERTEX_SHADER) {
    shaderTypeStr = "VERTEX";
  } else if(shaderType == GL_FRAGMENT_SHADER){
    shaderTypeStr = "FRAGMENT";
  }

  size_t shaderFileLength;
  FILE_HANDLE file;
  openFile(shaderPath, &file, &shaderFileLength);
  const char* shaderCodeCStr = fileBytes(file);

  u32 shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
  glCompileShader(shader);

  closeFile(file);

  s32 shaderSuccess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
  if (shaderSuccess != GL_TRUE)
  {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return shader;
}

void deleteShaderProgram(ShaderProgram* shaderProgram)
{
  // delete the shaders
  delete[] shaderProgram->vertexFileName;
  delete[] shaderProgram->fragmentFileName;

  glDeleteShader(shaderProgram->vertexShader);
  glDeleteShader(shaderProgram->fragmentShader);
  glDeleteProgram(shaderProgram->id);

  *shaderProgram = {}; // clear to zero
}

// utility uniform functions
inline void setUniform(GLuint shaderId, const std::string& name, bool val)
{
  glUniform1i(glGetUniformLocation(shaderId, name.c_str()), (int)val);
}

inline void setUniform(GLuint shaderId, const std::string& name, s32 val)
{
  glUniform1i(glGetUniformLocation(shaderId, name.c_str()), val);
}

inline void setUniform(GLuint shaderId, const std::string& name, u32 val)
{
  glUniform1ui(glGetUniformLocation(shaderId, name.c_str()), val);
}

inline void setUniform(GLuint shaderId, const std::string& name, f32 val)
{
  glUniform1f(glGetUniformLocation(shaderId, name.c_str()), val);
}

inline void setUniform(GLuint shaderId, const std::string& name, f32 val1, f32 val2)
{
  glUniform2f(glGetUniformLocation(shaderId, name.c_str()), val1, val2);
}

inline void setUniform(GLuint shaderId, const std::string& name, f32 val1, f32 val2, f32 val3)
{
  glUniform3f(glGetUniformLocation(shaderId, name.c_str()), val1, val2, val3);
}

inline void setUniform(GLuint shaderId, const std::string& name, f32 val1, f32 val2, f32 val3, f32 val4)
{
  glUniform4f(glGetUniformLocation(shaderId, name.c_str()), val1, val2, val3, val4);
}

inline void setUniform(GLuint shaderId, const std::string& name, const glm::mat4& val)
{
  glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()),
                     1, // count
                     GL_FALSE, // transpose: swap columns and rows (true or false)
                     glm::value_ptr(val)); // pointer to float values
}

inline void setUniform(GLuint shaderId, const std::string& name, const float* vals, const u32 arraySize)
{
  glUniform1fv(glGetUniformLocation(shaderId, name.c_str()), arraySize, vals);
}

inline void setUniform(GLuint shaderId, const std::string& name, const glm::vec2& val)
{
  setUniform(shaderId, name, val.x, val.y);
}

inline void setUniform(GLuint shaderId, const std::string& name, const glm::vec3& val)
{
  setUniform(shaderId, name, val.x, val.y, val.z);
}

inline void setUniform(GLuint shaderId, const std::string& name, const glm::vec4& val)
{
  setUniform(shaderId, name, val.x, val.y, val.z, val.w);
}

inline void setSamplerCube(GLuint shaderId, const std::string& name, GLint activeTextureIndex) {
  glUniform1i(glGetUniformLocation(shaderId, name.c_str()), activeTextureIndex);
}

inline void setSampler2D(GLuint shaderId, const std::string& name, GLint activeTextureIndex) {
  glUniform1i(glGetUniformLocation(shaderId, name.c_str()), activeTextureIndex);
}

inline void bindBlockIndex(GLuint shaderId, const std::string& name, u32 index)
{
  u32 blockIndex = glGetUniformBlockIndex(shaderId, name.c_str());
  glUniformBlockBinding(shaderId, blockIndex, index);
}