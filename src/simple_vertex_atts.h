#pragma once

/* CUBE VERTEX ATT DATA */
#define BottomLeftTexture 0.0f, 0.0f
#define BottomRightTexture 1.0f, 0.0f
#define TopLeftTexture 0.0f, 1.0f
#define TopRightTexture 1.0f, 1.0f
const u32 cubePosTexNormAttSizeInBytes = 8 * sizeof(f32);
const u32 cubePosNormTexNumElements = 12; // 2 triangles per side * 6 sides per cube
const f32 cubePosTexNormAttributes[] = {
        // positions           // normals            // texture positions
        // face #1
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  BottomRightTexture,    // bottom right
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  TopRightTexture,       // top right
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,  TopLeftTexture,        // top left
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,  BottomLeftTexture,     // bottom left
        // face #2
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
        0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
        0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
        // face #3
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   TopLeftTexture,        // top left
        0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   TopRightTexture,       // top right
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   BottomRightTexture,    // bottom right
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   BottomLeftTexture,     // bottom left
        // face #4
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   TopLeftTexture,        // top left
        0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   TopRightTexture,       // top right
        0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   BottomRightTexture,    // bottom right
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   BottomLeftTexture,      // bottom left
        // face #5
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   BottomLeftTexture,     // bottom left
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   BottomRightTexture,    // bottom right
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   TopRightTexture,       // top right
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   TopLeftTexture,        // top left
        // face #6
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    BottomLeftTexture,     // bottom left
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    BottomRightTexture,    // bottom right
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    TopRightTexture,       // top right
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    TopLeftTexture,        // top left
};
const f32 cubePosTexInvertedNormAttributes[] = { // Inverted normals and
        // positions           // normals            // texture positions
        // face #1			               -
        -0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
        -0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
        -0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
        -0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
        // face #2
        0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   BottomRightTexture,    // bottom right
        0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   TopRightTexture,       // top right
        0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   TopLeftTexture,        // top left
        0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   BottomLeftTexture,     // bottom left
        // face #3
        -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   TopLeftTexture,        // top left
        0.5f, -0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   TopRightTexture,       // top right
        0.5f, -0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   BottomRightTexture,    // bottom right
        -0.5f, -0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   BottomLeftTexture,     // bottom left
        // face #4
        -0.5f,  0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   TopLeftTexture,        // top left
        0.5f,  0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   TopRightTexture,       // top right
        0.5f,  0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   BottomRightTexture,    // bottom right
        -0.5f,  0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   BottomLeftTexture,      // bottom left
        // face #5
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  1.0f,   BottomLeftTexture,     // bottom left
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   BottomRightTexture,    // bottom right
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  1.0f,   TopRightTexture,       // top right
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  1.0f,   TopLeftTexture,        // top left
        // face #6
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, -1.0f,   BottomLeftTexture,     // bottom left
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f, -1.0f,   BottomRightTexture,    // bottom right
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f, -1.0f,   TopRightTexture,       // top right
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, -1.0f,   TopLeftTexture,        // top left
};
const u32 cubeAttributeIndices[]{ // 531024   // 420135
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

VertexAtt initializeCubePosNormTexVertexAttBuffers(bool invertNormals = false) {
  VertexAtt vertexAtt;
  vertexAtt.indexCount = ArrayCount(cubeAttributeIndices);
  vertexAtt.indexTypeSizeInBytes = sizeof(u32);
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  const f32* attributes = invertNormals ? cubePosTexInvertedNormAttributes : cubePosTexNormAttributes;

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER, // which buffer data is being entered in
               sizeof(cubePosTexNormAttributes), // size
               attributes,        // data
               GL_STATIC_DRAW); // GL_STATIC_DRAW (most likely not change), GL_DYNAMIC_DRAW (likely to change), GL_STREAM_DRAW (changes every time drawn)

  // position attribute
  glVertexAttribPointer(0, // position vertex attribute (used for location = 0 of Vertex Shader)
                        3, // size (we're using vec3)
                        GL_FLOAT, // type of data
                        GL_FALSE, // whether the data needs to be normalized
                        cubePosTexNormAttSizeInBytes, // vertex attribute stride
                        (void*)0); // offset
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        cubePosTexNormAttSizeInBytes,
                        (void*)(3 * sizeof(f32)));
  glEnableVertexAttribArray(1);

  // texture coords attribute
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        cubePosTexNormAttSizeInBytes,
                        (void*)(6 * sizeof(f32)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeAttributeIndices), cubeAttributeIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}


/* QUAD VERTEX ATT DATA */
const u32 quadIndices[]{
        0, 1, 2,
        0, 2, 3,
};

// ===== Quad values (vec3 pos, vec3 norm, vec2 tex) =====
const u32 quadPosNormTexVertexAttSizeInBytes = 8 * sizeof(f32);
const f32 quadPosNormTexVertexAttributes[] = {
        // positions		                // normal		                       // texCoords
        0.f,  1.f, 0.f,	0.f,  0.f, 1.f,	0.f, 1.f,
        0.f, 0.f, 0.f,	0.f,  0.f, 1.f,	0.f, 0.f,
        1.f, 0.f, 0.0,	0.f,  0.f, 1.f,	1.f, 0.f,
        1.f,  1.f, 0.f,	0.f,  0.f, 1.f,	1.f, 1.f,
};
// ===== Quad values (vec3 pos, vec3 norm, vec2 tex) =====



VertexAtt initializeQuadPosNormTexVertexAttBuffers() {
  VertexAtt vertexAtt;
  vertexAtt.indexCount = ArrayCount(quadIndices);
  vertexAtt.indexTypeSizeInBytes = sizeof(u32);
  glGenVertexArrays(1, &vertexAtt.arrayObject);
  glGenBuffers(1, &vertexAtt.bufferObject);
  glGenBuffers(1, &vertexAtt.indexObject);

  glBindVertexArray(vertexAtt.arrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexAtt.bufferObject);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(quadPosNormTexVertexAttributes),
               quadPosNormTexVertexAttributes,
               GL_STATIC_DRAW);

  // set the vertex attributes (position, normal, and texture)
  // position attribute
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexVertexAttSizeInBytes,
                        (void*)0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexVertexAttSizeInBytes,
                        (void*)(3 * sizeof(f32)));
  glEnableVertexAttribArray(1);

  // texture attribute
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        quadPosNormTexVertexAttSizeInBytes,
                        (void*)(6 * sizeof(f32)));
  glEnableVertexAttribArray(2);

  // bind element buffer object to give indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexAtt.indexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

  // unbind VBO, VAO, & EBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // Must unbind EBO AFTER unbinding VAO, since VAO stores all glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _) calls
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return vertexAtt;
}