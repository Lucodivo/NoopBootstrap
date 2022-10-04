#version 420

layout (binding = 2, std140) uniform DebugUBO {
  vec4 debugColor;
} ubo;

out vec4 FragColor;

void main()
{
  FragColor = ubo.debugColor;
}