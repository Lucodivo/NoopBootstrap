#version 420

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
  float x = (inPos.x) / (ubo.emulatedWindowRes.x * 0.5);
  float y = (inPos.y) / (ubo.emulatedWindowRes.y * 0.5);

  x *= ubo.spriteDimens.x;
  y *= ubo.spriteDimens.y;

  x += ubo.pos.x / (ubo.emulatedWindowRes.x * 0.5);
  y += ubo.pos.y / (ubo.emulatedWindowRes.y * 0.5);

  gl_Position = vec4(x, y, -1. /* minumum depth possible */, 1.);
  outNorm = inNorm;
  outTex = inTex;
}