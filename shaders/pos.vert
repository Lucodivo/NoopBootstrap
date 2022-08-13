#version 420

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inTex;

layout (binding = 0, std140) uniform UBO {
  mat4 model;
  mat4 view;
  mat4 projection;
} ubo;

layout (location = 0) out vec3 outNorm;
layout (location = 1) out vec2 outTex;

void main()
{
  gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos, 1.0);
  mat3 normalMat = mat3(transpose(inverse(ubo.model))); // Note: only necessary for non-uniform scaling
  outNorm = normalize(normalMat * inNorm);
  outTex = inTex;
}