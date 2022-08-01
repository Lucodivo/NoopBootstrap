#version 330 core

out vec4 FragColor;

uniform vec3 singleColor;

void main()
{
  FragColor = vec4(singleColor, 1.0);
}