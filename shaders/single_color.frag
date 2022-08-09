#version 420

layout (location = 0) in vec3 inNorm;
layout (location = 1) in vec2 inTex;

uniform sampler2D albedoTex;

out vec4 FragColor;

void main()
{
  vec4 albedoColor = texture(albedoTex, inTex);
  if(albedoColor.a < 0.5) { discard; }
  FragColor = albedoColor;
}