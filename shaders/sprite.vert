#version 420

// This vertex shader is designed to take in a quad with min pos of (0, 0, 0) and max pos of (1, 1, 0)

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inTex;

layout (binding = 1, std140) uniform UBO {
  ivec2 spriteDimens;
  ivec2 emulatedWindowRes;
  vec2 pos;
} ubo;

layout (location = 0) out vec3 outNorm;
layout (location = 1) out vec2 outTex;

void main()
{
  // goes from 0 to 1 in x and y coord
  vec2 normalizedScreenPos = ((inPos.xy * ubo.spriteDimens) + ubo.pos) / ubo.emulatedWindowRes;
  // goes from -1 to 1 in x and y coord
  vec4 ndcPos = vec4((2. * normalizedScreenPos.x) - 1., (2. * normalizedScreenPos.y) - 1., -1., 1.);

  gl_Position = ndcPos;
  outNorm = inNorm;
  outTex = inTex;
}