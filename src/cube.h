#pragma once

struct VertexAtt {
  GLuint arrayObject;
  GLuint bufferObject;
  GLuint indexObject;
};

const f32 cubePosAttributes[] = {
        // positions
        // face #1
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        // face #2
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        // face #3
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        // face #4
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // face #5
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        // face #6
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
};
const u32 cubePosAttributesSizeInBytes = (3/*position*/) * sizeof(f32);
const u32 cubeAttributeIndices[]{
        0, 1, 2, // 0
        2, 3, 0,
        4, 6, 5, // 1
        6, 4, 7,
        8, 9, 10, // 2
        10, 11, 8,
        12, 14, 13, // 3
        14, 12, 15,
        16, 18, 17, // 4
        18, 16, 19,
        20, 21, 22, // 5
        22, 23, 20,
};

VertexAtt initCubePosVertexAttBuffers() {
  VertexAtt vertexAtt;
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
               sizeof(cubePosAttributes), // size
               cubePosAttributes,        // data
               GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

  // position attribute
  glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader)
                        3, // size (we're using vec3)
                        GL_FLOAT, // type of data
                        GL_FALSE, // whether the data needs to be normalized
                        cubePosAttributesSizeInBytes, // vertex attribute stride
                        (void*)0); // offset
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeAttributeIndices), cubeAttributeIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}