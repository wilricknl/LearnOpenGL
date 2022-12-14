#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float horizontalOffset;

out vec4 vertexColor;

void main()
{
  gl_Position = vec4(aPos.x + horizontalOffset, aPos.y * -1.0f, aPos.z, 1.0f);
  vertexColor = vec4(aPos.x + horizontalOffset, aPos.y * -1.0f, aPos.z, 1.0f);
}
